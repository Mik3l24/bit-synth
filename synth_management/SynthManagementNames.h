//
// Created by micha on 24.11.2023.
//
#pragma once

#include <juce_core/juce_core.h>
// ID standard:
// == 0 = none
//  > 0 = Gate
//  < 0 = Oscillator
typedef int ConnectionID;
constexpr ConnectionID CONNECTION_NONE = 0;

namespace name
{
    // Subtree names
    const juce::Identifier OSCILLATOR("Oscillator");

    const juce::Identifier GATE_OR  ("GateOR");
    const juce::Identifier GATE_AND ("GateAND");
    const juce::Identifier GATE_NOT ("GateNOT");
    const juce::Identifier GATE_XOR ("GateXOR");

    const juce::Identifier MIX_CHANNEL ("MixChannel");

    // Parameter names
    const juce::Identifier ID("id");
    const juce::Identifier INDEX("index");
    const juce::Identifier RATIO("ratio");
    const juce::Identifier STARTING_PHASE("starting_phase");
    const juce::Identifier PULSE_WIDTH("pulse_width");

    const juce::Identifier CONNECTIONS("connections");

    const juce::Identifier LEVEL("level");

}

enum class ElementType
{
    OSCILLATOR,
    GATE,
    MIX_CHANNEL,
};

enum class GateType
{
    NONE, // Only exists so non-gate ElementAdderButtons don't need to be assigned a gate type
    NOT,
    AND, OR, XOR,
};


