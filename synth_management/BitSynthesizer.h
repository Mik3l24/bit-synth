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
        return size_t(processor_order[size_t(id - 1)]);
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
    void appendOscillator(const juce::ValueTree& tree);
    void appendMixChannel(const juce::ValueTree& tree);
    // Only for adding completely new gates, assumes its index is id - 1
    void appendGate(const juce::ValueTree& gate);
    // For placing gates when its index may be different from the default.
    // Assumes there is already enough space allocated in the used vectors
    // and will replace whatever was at its index!
    void placeGate(const juce::ValueTree& gate);

    void setInput(ElementID id, ConnectionID input_id, SubConnectionID sub_connection_id);
    void setInputs(const juce::ValueTree& element);

    void replaceProcessors(const std::vector<juce::ValueTree>& processors_to_replace);

public: // State methods
    void reconstructSynthFromTree(const juce::ValueTree& root);
protected:
    void reconstructConnectionsFromTree(const juce::ValueTree& root);

public: // Overrides
    void valueTreePropertyChanged(juce::ValueTree& affected_tree, const juce::Identifier& property) override;

    void valueTreeChildAdded(juce::ValueTree& parent_tree, juce::ValueTree& child_tree) override;

    void valueTreeChildRemoved(juce::ValueTree& parent_tree, juce::ValueTree& child_tree, int removed_child_i) override;

    void valueTreeRedirected(juce::ValueTree& affected_tree) override;

protected: // parameter members
    SynthStateManager state_manager;
    dsp::bitset zeroes; // Used for unconnected inputs, so that they don't have to check for that every time
    std::vector<ElementOrder> processor_order; // An indirection table. The parameter inside the ValueTree could've been used every time... but this is more convenient.

    double sample_rate = 44100.0;
    int samples_per_block = 512;
};


