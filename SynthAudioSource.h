//
// Created by micha on 13.11.2023.
//
#pragma once

#include <juce_audio_utils/juce_audio_utils.h>

class SynthAudioSource : public juce::AudioSource
{
public:
    SynthAudioSource(juce::MidiKeyboardState& keyState);
    ~SynthAudioSource() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

    void setUsingBitSynthSound();



private:
    juce::MidiKeyboardState& keyboardState;
    juce::Synthesiser synth;
};

