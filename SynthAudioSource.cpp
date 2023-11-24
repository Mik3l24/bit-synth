//
// Created by micha on 13.11.2023.
//
#include "SynthAudioSource.h"
#include "synth_management/SynthManagementNames.h"


SynthAudioSource::SynthAudioSource(juce::MidiKeyboardState& keyState)
    : keyboardState(keyState), synth(8)
{
    // Mock setup
    auto osc1 = synth.addOscillator();
    auto osc2 = synth.addOscillator();
    synth.setOscillatorRatio(osc2, 2.5);

    auto gate1 = synth.addGate(GateType::AND);
    synth.setGateInput(gate1, osc1, 0);
    synth.setGateInput(gate1, osc2, 1);

    auto ch1 = synth.addMixChannel();
    synth.setMixChannelInput(ch1, gate1);

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
