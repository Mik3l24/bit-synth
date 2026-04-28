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
    class Meta
    {
    public:
        Meta() = default;
        Meta(juce::ValueTree&& _generators, juce::ValueTree&& _components, juce::ValueTree&& _sinks)
            : generators(std::move(_generators)), components(std::move(_components)), sinks(std::move(_sinks)) {}
    public:
        // Turns out - these need to be here so BitSynthesiser can be persistently registered as a listener
        juce::ValueTree generators;
        juce::ValueTree components;
        juce::ValueTree sinks;
    };
    class Listener
    {
    public:
        virtual void stateReplaced() {}
    };
    class StateChangeSender
    {
    public:
        virtual void addStateListener(SynthStateManager::Listener* new_listener) {};
        virtual void removeStateListener(SynthStateManager::Listener* old_listener) {};
    };
public:
    explicit SynthStateManager(juce::AudioProcessorValueTreeState& _parameters, Meta& _meta)
        : parameters(_parameters), meta(_meta) {}
    SynthStateManager(const SynthStateManager&) = default;
    SynthStateManager(SynthStateManager&&) = default;


    ElementID addElementRep(ElementType element_type, GateType gate_type = GateType::NONE) const;

    void setConnection(ConnectionID source_id, ConnectionID target_id) const;
    void setElementPosition(ElementID id, ElementType element_type, juce::Point<int> position) const;


    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

private: // Internal methods
    [[nodiscard]] size_t getNextDynamicParameterID() const;
    void setNextFreeDynamicParameterID(size_t id) const;

    [[nodiscard]] juce::ValueTree newOscillatorRep(ElementID id) const;
    [[nodiscard]] juce::ValueTree newGateRep(ElementID id, GateType type) const;
    [[nodiscard]] juce::ValueTree newBitMixChannelRep(ElementID id) const;

    [[nodiscard]] juce::String registerDynamicParameter(juce::String friendly_name) const;

    [[nodiscard]] juce::ValueTree& getElementContainer(ElementType element_type) const;

public:
    juce::AudioProcessorValueTreeState& parameters;

private:
    Meta& meta;
};

