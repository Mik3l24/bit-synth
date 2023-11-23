//
// Created by micha on 13.11.2023.
//
#include "SynthAudioSource.h"
#include "dsp/BitSynthVoice.h"


SynthAudioSource::SynthAudioSource(juce::MidiKeyboardState& keyState)
    : keyboardState(keyState), synth(4)
{
    // Mock setup
    synth.addOscillator();
    synth.addOscillator();
    synth.setOscillatorRatio(-2, 2.5);

    synth.addGate(GateNodeRepresentation::Type::XOR);
    synth.setGateInput(1, -1, 0);
    synth.setGateInput(1, -2, 1);

    synth.addMixChannel();
    synth.setMixChannelInput(1, 1);

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
