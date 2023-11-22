//
// Created by micha on 08.11.2023.
//
#pragma once
#include <juce_audio_utils/juce_audio_utils.h>

#include <vector>
#include <memory>

#include "bitprocessing/Oscillator.h"
#include "bitprocessing/GateNode.h"
#include "bitprocessing/BitMixChannel.h"

template <typename T> using ptr = std::unique_ptr<T>;

class BitSynthSound : public juce::SynthesiserSound
{
public:
    BitSynthSound() = default;

    bool appliesToNote    (int) override        { return true; }
    bool appliesToChannel (int) override        { return true; }
};



class BitSynthVoice : public juce::SynthesiserVoice
{
public:
    BitSynthVoice() = default;

    bool canPlaySound(juce::SynthesiserSound* sound) override;

    void startNote(int midiNoteNumber, float velocity,
                   juce::SynthesiserSound*, int /*currentPitchWheelPosition*/) override;

    void stopNote(float /*velocity*/, bool allowTailOff) override;

    void pitchWheelMoved(int) override {};

    void controllerMoved(int, int) override {};

    void renderNextBlock(juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override;

public: // Parameter access methods
    void setMasterLevel(float level);

public:
    // Bit processing
    std::vector<ptr<Oscillator>> oscillators;
    std::vector<ptr<GateNode>> gates;
    std::vector<ptr<BitMixChannel>> bit_inputs;

protected:
    // Parameters
    float master_level = .125f;

private:
    // Voice processing
    float enveloped_level = 0.0;
    bool voice_active = false;
};

