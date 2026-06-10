//
// Created by micha on 08.11.2023.
//
#include "BitSynthVoice.h"

#include "synth_management/BitSynthSound.h"

using namespace dsp;

void BitSynthVoice::prepareToPlay(double sample_rate, int samples_per_block)
{
    for(auto& osc : oscillators)
        osc->prepareToPlay(sample_rate, samples_per_block);
    for(auto& gate : gates)
        gate->prepareToPlay(sample_rate, samples_per_block);
    for(auto& mix_channel : bit_inputs)
        mix_channel->prepareToPlay(sample_rate, samples_per_block);
}

bool BitSynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<BitSynthSound*>(sound) != nullptr;
}

void BitSynthVoice::startNote(int midi_note_number, float velocity, juce::SynthesiserSound*, int)
{
    voice_active = true;
    double pitch = juce::MidiMessage::getMidiNoteInHertz(midi_note_number);
    for(auto& osc : oscillators)
        osc->prepareVoice(pitch);
}

void BitSynthVoice::stopNote(float, bool allowTailOff)
{
    juce::ignoreUnused(allowTailOff);
    clearCurrentNote();
    voice_active = false;
}

void BitSynthVoice::renderNextBlock(juce::AudioSampleBuffer& output_buffer, int start_sample, int sample_n)
{
    // Stopping processing effectively functions as a gate envelope
    if(!voice_active)
        return;

    const int endSample = start_sample + sample_n;

    //// Bit processing ////
    // Process oscillators
    for(auto& osc : oscillators)
        osc->processBlock(Oscillator::uint(sample_n)); // Note - Oscillators currently align the block to the 0th bit

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
                if(gate->isReady())
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
                        // processBlock() no longer returns UNCONNECTED, so this case isn't used anymore
                        break;
                }
            }
        } while(!all_done && any_processing_done);

        // This occurs when a gate essentially requires itself to be processed due to a feedback loop.
        // Feedback could be interesting if delay was implemented, but for now, let's disallow this.
        if(!any_processing_done)
        {
            // However, a feedback loop can cause a crash, most likely from accessing the feedbacked gate's
            // uninitialized output by the mix channel, thereby processing of the block should be terminated.
            // Exceptions aren't used, because they seem to permanently disable audio till program restart.
            std::clog << "Error: BitSynthVoice::renderNextBlock() : Gate feedback loop\n";
            return;
        }
    }

    // Process mix channels to get floating point samples
    // This sample-wise loop is unavoidable, as floats need to be processed sample by sample.
    for(auto sample_index = start_sample; sample_index < endSample; sample_index++)
    {
        float sample = 0.0f;
        const auto sample_index_offset = sample_index - start_sample;
        for(auto& mix_channel: bit_inputs)
            // Unconnected channels merely return 0.0f, so we don't need to check for that.
            sample += mix_channel->getSample(sample_index_offset); // Has to be offset to account for the oscillators' allignment.
            // We could be accumulating DC offset here?
            // Limiting the max volume could also be useful, but not here.
            // oh, right, panning could be done here too!

        //// Floating point processing section ////
        // Or in other words, traditional synth elements

        // I'll need to read up on how to implement envelopes and perhaps later, filters.
        // For now, let's just use a simple gate envelope.

        sample *= master_level;

        // Saving to buffer
        for(auto channel_index = output_buffer.getNumChannels(); --channel_index >= 0;)
            output_buffer.addSample(channel_index, sample_index, sample);
    }

    // Reset gates
    for(auto& gate : gates)
        gate->resetStatus();

}

void BitSynthVoice::setMasterLevel(float level)
{
    master_level = level;
}
