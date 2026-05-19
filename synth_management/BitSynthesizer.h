//
// Created by micha on 15.11.2023.
//
#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <vector>

#include "SynthManagementNames.h"
#include "SynthStateManager.h"
#include "dsp/BitSynthVoice.h"
#include "dsp/bitprocessing/DspNames.h"
#include "dsp/bitprocessing/Gate.h"


class BitSynthesizer : public juce::Synthesiser,
                       public juce::ValueTree::Listener
{
public: // constructors
    explicit BitSynthesizer(int num_voices, const SynthStateManager& _state_manager);

public: // DSP-related methods
    void prepareToPlay(double sample_rate, int samples_per_block);

private:
    void prepareToPlayVoices();

protected: // Utility access methods
    inline size_t getProcessorIndex(ElementID id) const
    {
        // FUTURE - if we are to add sorting indirection this could be the place
        jassert(matchesSign(id, SIGN_PROCESSOR));
        id = std::abs(id);
        return size_t(id - 1);
    }
    inline size_t getGeneratorIndex(ElementID id) const
    {
        jassert(matchesSign(id, SIGN_GENERATOR));
        id = std::abs(id);
        return size_t(id - 1);
    }
    inline size_t getSinkIndex(ElementID id) const
    {
        jassert(matchesSign(id, SIGN_SINK));
        id = std::abs(id);
        return size_t(id - 1);
    }

protected: // Utility addition methods
    dsp::ptr<dsp::Gate> selectNewGate(const juce::ValueTree& gate);
    void addOscillator(const juce::ValueTree& tree);
    void addGate(const juce::ValueTree& gate);
    void addMixChannel(const juce::ValueTree& tree);

    void setInput(ElementID id, ConnectionID input_id, SubConnectionID sub_connection_id);
    void setInputs(const juce::ValueTree& element);

public: // State methods
    void reconstructSynthFromTree(juce::ValueTree& root);

public: // Overrides
    void valueTreePropertyChanged(juce::ValueTree& affected_tree, const juce::Identifier& property) override;

    void valueTreeChildAdded(juce::ValueTree& parent_tree, juce::ValueTree& child_tree) override;

    void valueTreeChildRemoved(juce::ValueTree& parent_tree, juce::ValueTree& child_tree, int removed_child_i) override;

    void valueTreeRedirected(juce::ValueTree& affected_tree) override;

protected: // parameter members
    SynthStateManager state_manager;
    dsp::bitset zeroes; // Used for unconnected inputs, so that they don't have to check for that every time
    double sample_rate = 44100.0;
    int samples_per_block = 512;
};


