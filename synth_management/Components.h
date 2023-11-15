//// Components.h ////
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



struct OscillatorTemplate
{
    ConnectionID id;
    double ratio = 1.0;
    double starting_phase = 0.0;
    double pulse_width = 0.5;

    OscillatorTemplate(ConnectionID id)
            : id(id) {}

    OscillatorTemplate(ConnectionID id, double ratio, double starting_phase, double pulse_width)
            : id(id), ratio(ratio), starting_phase(starting_phase), pulse_width(pulse_width) {}

};


struct GateNodeTemplate
{
    enum class Type
    {
        NOT,
        AND, OR, XOR,
    };
    ConnectionID id;
    const Type type;
    const size_t num_inputs = 2;
    ConnectionID input_ids[2] = {0, 0};

    GateNodeTemplate(ConnectionID id, Type type, size_t num_inputs)
            : id(id), type(type), num_inputs(num_inputs) {}
    GateNodeTemplate(ConnectionID id, Type type, ConnectionID input_ids[2])
            : id(id), type(type), input_ids{input_ids[0], input_ids[1]} {}
    GateNodeTemplate(ConnectionID id, ConnectionID input_id)  // Special case for NOT... until I add a buffer gate?
            : id(id), type(Type::NOT), num_inputs(1), input_ids{input_id} {}
};


struct BitMixChannelTemplate
{
    ConnectionID id;
    ConnectionID input_id = 0;
    float level = .5f;

    BitMixChannelTemplate(ConnectionID id)
            : id(id) {}
    BitMixChannelTemplate(ConnectionID id, ConnectionID input_id, float level)
            : id(id), input_id(input_id), level(level) {}
};
