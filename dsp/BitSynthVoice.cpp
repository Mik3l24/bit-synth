//
// Created by micha on 08.11.2023.
//
#include "BitSynthVoice.h"


bool BitSynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<BitSynthSound*> (sound) != nullptr;
}

void BitSynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound*, int)
{
    voice_active = true;
    double pitch = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    for(auto& osc : oscillators)
        osc->prepareVoice(pitch);
}

void BitSynthVoice::stopNote(float, bool allowTailOff)
{
    clearCurrentNote();
    voice_active = false;
}

void BitSynthVoice::renderNextBlock(juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples)
{
    if(!voice_active)
        return;

    const int endSample = startSample + numSamples;

    // Prepare
    // Should be preferably done in prepareToPlay(), but neither SynthesiserVoice nor Synthesiser provides that.
    // TODO: Make a custom Synthesiser class with prepareToPlay().
    for(auto& osc : oscillators)
        osc->prepareToPlay(outputBuffer.getNumSamples(), getSampleRate());

    for(auto& gate : gates)
        gate->prepareToPlay(outputBuffer.getNumSamples());

    for(auto& mix_channel : bit_inputs)
        mix_channel->prepareToPlay(outputBuffer.getNumSamples());

    //// Bit processing ////
    // Process oscillators
    for(auto sample_index = startSample; sample_index < endSample; sample_index++)
        for(auto& osc : oscillators)
            osc->processSample(sample_index);

    // Process gates
    if(!gates.empty())
    {
        bool any_processing_done; // Guard against infinite loops due to recursion
        bool all_done;
        do {
            any_processing_done = false;
            all_done = true;
            for(auto& gate : gates) // Gates should be sorted to minimize loop repeats
            {
                // Skip gates
                // Is skipping gates faster than processing them?
                // or would popping them from a queue be better?
                // Quick fix: clearing the status of gates unconnected by proxy is currently only done in processBlock,
                // so we can't skip them
                if(gate->isReady() /*|| gate->isUnconnected()*/) // Quick fix
                    continue;
                switch(gate->processBlock())
                {
                    case status::SUCCESS:
                        any_processing_done = true;
                        break;
                    case status::POSTPONED:
                        all_done = false;
                        break;
                    case status::UNCONNECTED:
                        // processBlock() already handles propagation of unconnected gates
                        break;
                }
            }
        } while(!all_done && any_processing_done);

        // This occurs when a gate essentially requires itself to be processed due to a feedback loop.
        // Feedback could be interesting if delay was implemented, but for now, let's disallow this.
        if(!any_processing_done)
        {
            if(std::all_of( // the gates are unconnected
                gates.begin(), gates.end(),
                [](const auto& gate)->bool
                    { return gate->isUnconnected(); }))
            {
                // Gates being unconnected isn't harmful and might not even warrant a warning if an oscillator
                // is connected directly to a mix channel, as the synth will still be audible.
                // However, it could be a sign of a mistake, so let's warn the user.
                std::clog << "Warning: BitSynthVoice::renderNextBlock() : All gates are unconnected\n";
            }
            else
            {
                // However, a feedback loop can cause a crash, most likely from accessing the feedbacked gate's
                // uninitialized output by the mix channel, thereby processing of the block should be terminated.
                // Exceptions aren't used, because they seem to permanently disable audio till program restart.
                std::clog << "Error: BitSynthVoice::renderNextBlock() : Gate feedback loop\n";
                return;
            }
        }
    }

    // Process mix channels to get floating point samples
    //  I don't think there is a way to avoid having two sample-wise loops.
    for(auto sample_index = startSample; sample_index < endSample; sample_index++)
    {
        float sample = 0.0f;
        for(auto& mix_channel: bit_inputs)
            // Unconnected channels merely return 0.0f, so we don't need to check for that.
            sample += mix_channel->getSample(sample_index);
            // We could be accumulating DC offset here?
            // Limiting the max volume could also be useful, but not here.
            // oh, right, panning could be done here too!

        //// Floating point processing section ////
        // Or in other words, traditional synth elements

        // I'll need to read up on how to implement envelopes and perhaps later, filters.
        // For now, let's just use a simple gate envelope.

        sample *= master_level;

        // Saving to buffer
        for(auto channel_index = outputBuffer.getNumChannels(); --channel_index >= 0;)
            outputBuffer.addSample(channel_index, sample_index, sample);
    }

    // Reset gates
    for(auto& gate : gates)
        gate->resetStatus();

}

void BitSynthVoice::setMasterLevel(float level)
{
    master_level = level;
}
