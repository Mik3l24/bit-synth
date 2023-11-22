//// RepresentationStructs.h ////
// Structs for representing bitprocessing components to be constructed in BitSynthVoice
//
// Created by micha on 15.11.2023.
//
#pragma once

// ID standard:
// == 0 = none
//  > 0 = Gate
//  < 0 = Oscillator
typedef int ConnectionID;

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

    OscillatorRepresentation(ConnectionID id, double ratio, double starting_phase, double pulse_width)
            : SourceRepresentation(id, -id - 1), ratio(ratio), starting_phase(starting_phase), pulse_width(pulse_width) {}

};


struct GateNodeRepresentation : public SourceRepresentation
{
    enum class Type
    {
        NOT,
        AND, OR, XOR,
    };
    const Type type;
    const size_t num_inputs = 2;
    ConnectionID input_ids[2] = {0, 0};

    GateNodeRepresentation(ConnectionID id, Type type, size_t num_inputs)
            : SourceRepresentation(id, id - 1), type(type), num_inputs(num_inputs) {}
    GateNodeRepresentation(ConnectionID id, Type type, ConnectionID input_ids[2])
            : SourceRepresentation(id, id - 1), type(type), input_ids{input_ids[0], input_ids[1]} {}
    GateNodeRepresentation(ConnectionID id, ConnectionID input_id)  // Special case for NOT... until I add a buffer gate?
            : SourceRepresentation(id, id - 1), type(Type::NOT), num_inputs(1), input_ids{input_id} {}
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
