//// RepresentationStructs.h ////
// Structs for representing bitprocessing components to be constructed in BitSynthVoice
//
// Created by micha on 15.11.2023.
//
#pragma once

#include <juce_data_structures/juce_data_structures.h>
#include "SynthManagementNames.h"

inline juce::ValueTree newOscillatorRep(ConnectionID id)
{
    return juce::ValueTree(Name::OSCILLATOR,
        {
            {Name::ID,             id},
            {Name::INDEX,          -id-1},
            {Name::RATIO,          1.0},
            {Name::STARTING_PHASE, 0.0},
            {Name::PULSE_WIDTH,    0.5f}
        }
    );
}

inline juce::ValueTree newGateRep(ConnectionID id, ElementType type)
{
    const juce::Identifier* type_name = nullptr;
    switch(type)
    {
        case ElementType::GATE_NOT:
            type_name = &Name::GATE_NOT;
            break;
        case ElementType::GATE_AND:
            type_name = &Name::GATE_AND;
            break;
        case ElementType::GATE_OR:
             type_name = &Name::GATE_OR;
             break;
        case ElementType::GATE_XOR:
            type_name = &Name::GATE_XOR;
            break;
        default:
            jassertfalse; // Invalid gate type
    }
    return juce::ValueTree(*type_name,
        {
            {Name::ID, id},
            {Name::INDEX, id-1},
            {Name::CONNECTIONS,
                type == ElementType::GATE_NOT
                ? juce::Array<juce::var>(CONNECTION_NONE)
                : juce::Array<juce::var>({CONNECTION_NONE, CONNECTION_NONE})
            }
        }
     );
}

inline juce::ValueTree newBitMixChannelRep(ConnectionID id)
{
    return juce::ValueTree(Name::MIX_CHANNEL,
        {
            {Name::ID, id},
            {Name::INDEX, id-1},
            {Name::CONNECTIONS, CONNECTION_NONE},
            {Name::LEVEL, 0.5f}
        }
    );
}

// Deprecated structs
#if false
struct IdentifiableRepresentation
{
    ConnectionID id;
    ConnectionID index;

    explicit IdentifiableRepresentation(ConnectionID id, ConnectionID index)
            : id(id), index(index) {}
};


struct SourceRepresentation : public IdentifiableRepresentation
{
    explicit SourceRepresentation(ConnectionID id, ConnectionID index)
            : IdentifiableRepresentation(id, index) {}
};


struct OscillatorRepresentation : public SourceRepresentation
{
    double ratio = 1.0;
    double starting_phase = 0.0;
    float pulse_width = 0.5f;

    explicit OscillatorRepresentation(ConnectionID id)
            : SourceRepresentation(id, -id - 1) {}

    OscillatorRepresentation(ConnectionID id, double ratio, double starting_phase, float pulse_width)
            : SourceRepresentation(id, -id - 1), ratio(ratio), starting_phase(starting_phase), pulse_width(pulse_width) {}

};


struct GateNodeRepresentation : public SourceRepresentation
{
    const ElementType type;
    const size_t num_inputs = 2;
    ConnectionID input_ids[2] = {0, 0};

    GateNodeRepresentation(ConnectionID id, ElementType type, size_t num_inputs)
            : SourceRepresentation(id, id - 1), type(type), num_inputs(num_inputs) {}
    GateNodeRepresentation(ConnectionID id, ElementType type, ConnectionID input_ids[2])
            : SourceRepresentation(id, id - 1), type(type), input_ids{input_ids[0], input_ids[1]} {}
    GateNodeRepresentation(ConnectionID id, ConnectionID input_id)  // Special case for NOT... until I add a buffer gate?
            : SourceRepresentation(id, id - 1), type(ElementType::GATE_NOT), num_inputs(1), input_ids{input_id} {}
};


struct BitMixChannelRepresentation : public IdentifiableRepresentation
{
    ConnectionID input_id = 0;
    float level = .5f;

    explicit BitMixChannelRepresentation(ConnectionID id)
            : IdentifiableRepresentation(id, id - 1) {}
    BitMixChannelRepresentation(ConnectionID id, ConnectionID input_id, float level)
            : IdentifiableRepresentation(id, id - 1), input_id(input_id), level(level) {}
};
#endif