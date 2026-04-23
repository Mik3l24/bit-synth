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

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override          { return new ui::SynthEditor(*this, state_manager); }
    bool hasEditor() const override                              { return true; }

    //==============================================================================
    const juce::String getName() const override                  { return "Boolean Synthesizer"; }
    bool acceptsMidi() const override                            { return true; }
    bool producesMidi() const override                           { return false; }
    double getTailLengthSeconds() const override                 { return 0; } // I suspect this may have to be changed if envelopes were implemented

    //==============================================================================
    int getNumPrograms() override                                { return 1; }
    int getCurrentProgram() override                             { return 0; }
    void setCurrentProgram(int) override                         {}
    const juce::String getProgramName(int) override              { return {}; }
    void changeProgramName(int, const juce::String&) override    {}
    
    void getStateInformation(juce::MemoryBlock& dest_data) override;
    void setStateInformation(const void* data, int size_in_bytes) override;


    void prepareToPlay (double, int) override;
    void releaseResources() override;
    void processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midi) override;

private:
    void initState();

private:
    juce::AudioProcessorValueTreeState parameters;
    SynthStateManager state_manager;
    SynthStateManager::ManagmentState state_manager_state;

    std::unique_ptr<BitSynthesizer> bit_synth;
    const int num_voices = 8; // If we add settings for changing this, change to non-const.


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SynthAudioProcessor);
};


