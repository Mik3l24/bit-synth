//
// Created by micha on 15.04.2026.
//

#pragma once

#include <juce_data_structures/juce_data_structures.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "SynthManagementNames.h"

// A wrapper around a reference to juce::AudioProcessorValueTreeState,
// providing methods that make sense in this synthesizer w/o making a subclass of it.
// It's very much copyable, like juce::ValueTree.
class SynthStateManager
{
public:
    class ManagmentState
    {
    public:
        size_t next_free_dynamic_parameter_id;
    };
public:
    explicit SynthStateManager(juce::AudioProcessorValueTreeState& parameters, ManagmentState& state)
        : parameters(parameters), state(state) {}
    SynthStateManager(const SynthStateManager&) = default;
    SynthStateManager(SynthStateManager&&) = default;



    ElementID addElementRep(ElementType element_type, GateType gate_type = GateType::NONE);

    void setConnection(ConnectionID source_id, ConnectionID target_id);


    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

private:
    juce::ValueTree newOscillatorRep(ElementID id);
    juce::ValueTree newGateRep(ElementID id, GateType type);
    juce::ValueTree newBitMixChannelRep(ElementID id);

    juce::String registerDynamicParameter(juce::String friendly_name);

public:
    juce::AudioProcessorValueTreeState& parameters;

private:
    ManagmentState& state;
};

