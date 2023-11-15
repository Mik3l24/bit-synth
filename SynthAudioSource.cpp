//
// Created by micha on 13.11.2023.
//
#include "SynthAudioSource.h"
#include "BitSynthVoice.h"


SynthAudioSource::SynthAudioSource(juce::MidiKeyboardState& keyState)
    : keyboardState(keyState)
{
    auto voice = new BitSynthVoice(); // We shouldn't retain it, so it's a temporary measure to set it up
    // Perhaps the "SynthesiserSound" can be a template for the structure of the voice?

    // Mock setup
    {
        auto& oscillators = voice->oscillators;
        for(int i = 0; i < 3; i++)
            oscillators.emplace_back(new Oscillator());
        oscillators[1]->setRatio(2.5);
        oscillators[2]->setPulseWidth(0.75);

        auto& gates = voice->gates;
        gates.emplace_back(new XorGate());
        gates.emplace_back(new AndGate());
        gates.emplace_back(new NotGate());
        gates.emplace_back(new OrGate()); //will be unconnected

        gates[0]->setInput(oscillators[0].get(), 0);
        gates[0]->setInput(oscillators[1].get(), 1);
        gates[2]->setInput(gates[0].get());
        gates[1]->setInput(gates[2].get(), 0);
        gates[1]->setInput(oscillators[2].get(), 1);
        gates[3]->setInput(gates[0].get(), 0);
        // no input for gates[3][1]

        auto& mixer = voice->bit_inputs;
        mixer.emplace_back(new BitMixChannel());
        mixer.emplace_back(new BitMixChannel());
        mixer.emplace_back(new BitMixChannel());

        mixer[0]->setInput(oscillators[0].get());
        mixer[0]->setLevel(.25);
        mixer[1]->setInput(gates[1].get());
        mixer[1]->setLevel(.065);
        mixer[2]->setInput(gates[3].get());
        mixer[2]->setLevel(.065);

    }

    synth.addSound(new BitSynthSound());
    synth.addVoice(voice);
}

SynthAudioSource::~SynthAudioSource()
{
    synth.clearSounds();
}

void SynthAudioSource::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);
}

void SynthAudioSource::releaseResources()
{
}

void SynthAudioSource::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();

    juce::MidiBuffer incomingMidi;
    keyboardState.processNextMidiBuffer(incomingMidi, bufferToFill.startSample,
                                        bufferToFill.numSamples, true);

    synth.renderNextBlock(*bufferToFill.buffer, incomingMidi,
                          bufferToFill.startSample, bufferToFill.numSamples);
}
