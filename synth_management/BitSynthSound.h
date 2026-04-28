#pragma once

#include <juce_audio_utils/juce_audio_utils.h>

class BitSynthSound : public juce::SynthesiserSound
{
public: // Methods
    BitSynthSound() = default;

    bool appliesToNote(int)    override { return true; }
    bool appliesToChannel(int) override { return true; }

};
