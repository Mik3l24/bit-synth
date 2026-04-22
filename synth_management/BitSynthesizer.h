//
// Created by micha on 15.11.2023.
//
#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include <vector>

#include "RepresentationStructs.h"
#include "SynthManagementNames.h"
#include "SynthStateManager.h"


class BitSynthesizer : public juce::Synthesiser,
                       public juce::ValueTree::Listener
{
public: // constructors
    explicit BitSynthesizer(int num_voices, const SynthStateManager& state_manager);

protected: // Utility access methods
    juce::ValueTree getOscillator(ConnectionID id) const;

    juce::ValueTree getGate(ConnectionID id) const;

    juce::ValueTree getMixChannel(ConnectionID id) const;

    juce::ValueTree getSource(ConnectionID id) const;

    inline int getComponentIndex(ElementID id) const
    {
        // FUTURE - if we are to add sorting indirection this could be the place
        jassert(matchesSign(id, SIGN_COMPONENT));
        id = std::abs(id);
        return id - 1;
    }
    inline int getGeneratorIndex(ElementID id) const
    {
        jassert(matchesSign(id, SIGN_GENERATOR));
        id = std::abs(id);
        return id - 1;
    }
    inline int getSinkIndex(ElementID id) const
    {
        jassert(matchesSign(id, SIGN_SINK));
        id = std::abs(id);
        return id - 1;
    }

protected: // Utility addition methods
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
#if false
    std::vector<OscillatorRepresentation> oscillators;
    std::vector<GateNodeRepresentation> gates;
    std::vector<BitMixChannelRepresentation> mix_channels;
#endif
    // Other per-voice parameters
    float master_level = .125f;

    SynthStateManager state_manager;

};


