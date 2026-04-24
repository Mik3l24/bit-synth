//
// Created by micha on 12.04.2026.
//

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#include "ui/SynthEditor.h"
#include "synth_management/BitSynthesizer.h"
#include "synth_management/SynthStateManager.h"


class SynthAudioProcessor : public juce::AudioProcessor
{
public:
    SynthAudioProcessor();
    ~SynthAudioProcessor();

    // Graphical editor overrides
    juce::AudioProcessorEditor* createEditor() override          { return new ui::SynthEditor(*this, state_manager); }
    bool hasEditor() const override                              { return true; }

    // General information overrides
    const juce::String getName() const override                  { return "Boolean Synthesizer"; }
    bool acceptsMidi() const override                            { return true; }
    bool producesMidi() const override                           { return false; }
    double getTailLengthSeconds() const override                 { return 0; } // I suspect this may have to be changed if envelopes were implemented

    // Program management overrides (not used currently)
    int getNumPrograms() override                                { return 1; }
    int getCurrentProgram() override                             { return 0; }
    void setCurrentProgram(int) override                         {}
    const juce::String getProgramName(int) override              { return {}; }
    void changeProgramName(int, const juce::String&) override    {}

    // State management overrides
    void getStateInformation(juce::MemoryBlock& dest_data) override;
    void setStateInformation(const void* data, int size_in_bytes) override;

    // Audio processing overrides
    void prepareToPlay(double sample_rate, int max_samples_per_block) override;
    void releaseResources() override;
    void processBlock(juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midi) override;

private: // Internal methods
    void initState();

private: // Members
    juce::AudioProcessorValueTreeState parameters;
    SynthStateManager state_manager;
    SynthStateManager::Meta internal_state;

    std::unique_ptr<ui::SynthEditor> editor;

    std::unique_ptr<BitSynthesizer> bit_synth;
    const int num_voices = 8; // If we add settings for changing this, change to non-const.


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthAudioProcessor);
};


