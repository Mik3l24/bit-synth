//
// Created by micha on 15.11.2023.
//

#include "BitSynthesizer.h"
#include "../dsp/BitSynthVoice.h"
#include "SynthManagementNames.h"

inline BitSynthVoice* castVoice(juce::SynthesiserVoice* voice)
{
    auto* bit_voice = dynamic_cast<BitSynthVoice*>(voice);
    if(bit_voice == nullptr)
        throw std::runtime_error("BitSynthesizer::addOscillator: Voice is not a BitSynthVoice!");
    return bit_voice;
}


BitSynthesizer::BitSynthesizer(int num_voices)
{
    addSound(new BitSynthSound());
    for(int i = 0; i < num_voices; i++)
        addVoice(new BitSynthVoice());
}


OscillatorRepresentation& BitSynthesizer::getOscillator(ConnectionID id)
{
    id = -id; // Convert to positive id, as oscillators use negative ids
    if(id <= 0 || id > oscillators.size())
        throw std::runtime_error("BitSynthesizer::getOscillator: Invalid id!");
    return oscillators[id - 1];
}

GateNodeRepresentation& BitSynthesizer::getGate(ConnectionID id)
{
    if(id <= 0 || id > gates.size())
        throw std::runtime_error("BitSynthesizer::getGate: Invalid id!");
    return gates[id - 1];
}

BitMixChannelRepresentation& BitSynthesizer::getMixChannel(ConnectionID id)
{
    if(id <= 0 || id > mix_channels.size())
        throw std::runtime_error("BitSynthesizer::getMixChannel: Invalid id!");
    return mix_channels[id - 1];
}


SourceRepresentation* BitSynthesizer::getSource(ConnectionID id)
{
    if(id == 0)
        return nullptr;
    else if(id > 0)
        return &gates[id - 1];
    else
        return &oscillators[-id - 1];
}


ConnectionID BitSynthesizer::addOscillator()
{
    // Add representation
    ConnectionID id = -(ConnectionID(oscillators.size()) + 1);
    oscillators.emplace_back(id);
    // Add the new oscillator to all voices
    for(auto& voice : voices)
    {
        auto* bit_voice = castVoice(voice);

        bit_voice->oscillators.emplace_back(new Oscillator());
    }
    return id;
}

void BitSynthesizer::setOscillatorRatio(ConnectionID id, double ratio)
{
    OscillatorRepresentation& osc = getOscillator(id);
    osc.ratio = ratio;
    for(auto& voice : voices)
    {
        auto* bit_voice = castVoice(voice);

        bit_voice->oscillators[osc.index]->setRatio(osc.ratio);
    }
}

void BitSynthesizer::setOscillatorStartingPhase(ConnectionID id, double starting_phase)
{
    OscillatorRepresentation& osc = getOscillator(id);
    osc.starting_phase = starting_phase;
    for(auto& voice : voices)
    {
        auto* bit_voice = castVoice(voice);

        bit_voice->oscillators[osc.index]->setStartingPhase(osc.starting_phase);
    }
}

void BitSynthesizer::setOscillatorPulseWidth(ConnectionID id, float pulse_width)
{
    OscillatorRepresentation& osc = getOscillator(id);
    osc.pulse_width = pulse_width;
    for(auto& voice : voices)
    {
        auto* bit_voice = castVoice(voice);

        bit_voice->oscillators[osc.index]->setPulseWidth(osc.pulse_width);
    }
}

inline GateNode* selectNewGate(GateNodeRepresentation& gate)
{
    switch(gate.type)
    {
        case GateType::NOT:
            return new NotGate();
        case GateType::AND:
            return new AndGate();
        case GateType::OR:
            return new OrGate();
        case GateType::XOR:
            return new XorGate();
    }
}

ConnectionID BitSynthesizer::addGate(GateType type)
{
    // Add representation
    ConnectionID id = ConnectionID(gates.size()) + 1;
    gates.emplace_back(id, type, type == GateType::NOT ? 1 : 2);
    GateNodeRepresentation& gate = gates.back();
    // Add the new gate to all voices
    for(auto& voice : voices)
    {
        auto* bit_voice = castVoice(voice);

        bit_voice->gates.emplace_back(selectNewGate(gate));
    }
    return id;
}

void BitSynthesizer::setGateInput(ConnectionID id, ConnectionID input_id, size_t input_index)
{
    GateNodeRepresentation& gate = getGate(id);
    if(input_index >= gate.num_inputs)
        throw std::runtime_error("BitSynthesizer::setGateInput: Invalid input index!");
    gate.input_ids[input_index] = input_id;
    auto* source = getSource(gate.input_ids[input_index]);

    for(auto& voice : voices)
    {
        auto* bit_voice = castVoice(voice);

        BitSource* bit_source = nullptr;
        if(source->id > 0)
            bit_source = bit_voice->gates[source->index].get();
        else if(source->id < 0)
            bit_source = bit_voice->oscillators[source->index].get();

        bit_voice->gates[gate.index]->setInput(bit_source, input_index);
    }
}

ConnectionID BitSynthesizer::addMixChannel()
{
    // Add representation
    ConnectionID id = ConnectionID(mix_channels.size()) + 1;
    mix_channels.emplace_back(id);
    // Add the new channel to all voices
    for(auto& voice : voices)
    {
        auto* bit_voice = castVoice(voice);

        bit_voice->bit_inputs.emplace_back(new BitMixChannel());
    }
    return id;
}

void BitSynthesizer::setMixChannelInput(ConnectionID id, ConnectionID input_id)
{
    BitMixChannelRepresentation& channel = getMixChannel(id);
    channel.input_id = input_id;
    auto* source = getSource(channel.input_id);

    for(auto& voice : voices)
    {
        auto* bit_voice = castVoice(voice);

        BitSource* bit_source = nullptr;
        if(source->id > 0)
            bit_source = bit_voice->gates[source->index].get();
        else if(source->id < 0)
            bit_source = bit_voice->oscillators[source->index].get();

        bit_voice->bit_inputs[channel.index]->setInput(bit_source);
    }
}

void BitSynthesizer::setMixChannelLevel(ConnectionID id, float level)
{
    BitMixChannelRepresentation& channel = getMixChannel(id);
    channel.level = level;
    for(auto& voice : voices)
    {
        auto* bit_voice = castVoice(voice);

        bit_voice->bit_inputs[channel.index]->setLevel(channel.level);
    }
}

void BitSynthesizer::setMasterLevel(float level)
{
    master_level = level;
    for(auto& voice : voices)
    {
        auto* bit_voice = castVoice(voice);

        bit_voice->setMasterLevel(master_level);
    }
}






