//
// Created by micha on 15.11.2023.
//

#include "BitSynthesizer.h"
#include "dsp/BitSynthVoice.h"
#include "BitSynthSound.h"
#include "SynthManagementNames.h"

inline BitSynthVoice* castVoice(juce::SynthesiserVoice* voice)
{
    auto* bit_voice = static_cast<BitSynthVoice*>(voice); // Should be always correct, as only BitSynthVoices are added to the synthesizer
    jassert(bit_voice != nullptr);
    return bit_voice;
}

inline BitSynthSound* castSound(juce::SynthesiserSound* sound)
{
    auto* bit_sound = static_cast<BitSynthSound*>(sound); // Should be always correct, as only BitSynthSounds are added to the synthesizer
    jassert(bit_sound != nullptr);
    return bit_sound;
}

#define CAST_VOICE(voice) auto* bit_voice = castVoice(voice)
#define CAST_SOUND jassert(sounds.size() == 1); auto* bit_sound = castSound(sounds.getFirst())


inline juce::Array<juce::var> connections(const juce::ValueTree& tree)
{
    return *tree[name::CONNECTIONS].getArray();
}


BitSynthesizer::BitSynthesizer(int num_voices)
{
    addSound(new BitSynthSound());
    for(int i = 0; i < num_voices; i++)
        addVoice(new BitSynthVoice());
}


juce::ValueTree BitSynthesizer::getOscillator(ConnectionID id) const
{
    CAST_SOUND;
    id = -id; // Convert to positive id, as oscillators use negative ids
    jassert(id > 0 && id <= bit_sound->sources.getNumChildren());
    return bit_sound->sources.getChild(id - 1);
}

juce::ValueTree BitSynthesizer::getGate(ConnectionID id) const
{
    CAST_SOUND;
    jassert(id > 0 && id <= bit_sound->components.getNumChildren());
    return bit_sound->components.getChild(id - 1);
}

juce::ValueTree BitSynthesizer::getMixChannel(ConnectionID id) const
{
    CAST_SOUND;
    jassert(id > 0 && id <= bit_sound->sinks.getNumChildren());
    return bit_sound->sinks.getChild(id - 1);
}


juce::ValueTree BitSynthesizer::getSource(ConnectionID id) const
{
    if(id == 0)
        return juce::ValueTree(); // Return an invalid ValueTree for the "no connection" case
    else if(id > 0)
        return getGate(id);
    else
        return getOscillator(id);
}


ConnectionID BitSynthesizer::addOscillator()
{
    CAST_SOUND;
    const ConnectionID id = -(ConnectionID(bit_sound->sources.getNumChildren()) + 1); // FUTURE May need to be handled differently for multi-output sources

    bit_sound->sources.appendChild(newOscillatorRep(id), bit_sound->undo_manager);

    for(const auto& voice : voices)
    {
        CAST_VOICE(voice);
        bit_voice->oscillators.emplace_back(new Oscillator());
    }
    return id;
}

void BitSynthesizer::setOscillatorRatio(ConnectionID id, double ratio)
{
    CAST_SOUND;
    auto osc = getOscillator(id);
    osc.setProperty(name::RATIO, ratio, bit_sound->undo_manager);

    for(auto& voice : voices)
    {
        auto* bit_voice = castVoice(voice);

        bit_voice->oscillators[ConnectionID(osc[name::INDEX])]->setRatio(osc[name::RATIO]);
    }
}

void BitSynthesizer::setOscillatorStartingPhase(ConnectionID id, double starting_phase)
{
    CAST_SOUND;
    auto osc = getOscillator(id);
    osc.setProperty(name::STARTING_PHASE, starting_phase, bit_sound->undo_manager);

    for(auto& voice : voices)
    {
        auto* bit_voice = castVoice(voice);

        bit_voice->oscillators[ConnectionID(osc[name::INDEX])]->setStartingPhase(osc[name::STARTING_PHASE]);
    }
}

void BitSynthesizer::setOscillatorPulseWidth(ConnectionID id, float pulse_width)
{
    CAST_SOUND;
    auto osc = getOscillator(id);
    osc.setProperty(name::PULSE_WIDTH, pulse_width, bit_sound->undo_manager);

    for(auto& voice : voices)
    {
        auto* bit_voice = castVoice(voice);

        bit_voice->oscillators[ConnectionID(osc[name::INDEX])]->setPulseWidth(osc[name::PULSE_WIDTH]);
    }
}

inline ptr<GateNode> selectNewGate(juce::ValueTree gate)
{
    const auto type = gate.getType();
    if(type == name::GATE_NOT) return ptr<GateNode>(new NotGate());
    if(type == name::GATE_AND) return ptr<GateNode>(new AndGate());
    if(type == name::GATE_OR)  return ptr<GateNode>(new OrGate());
    if(type == name::GATE_XOR) return ptr<GateNode>(new XorGate());
    jassertfalse;
}

ConnectionID BitSynthesizer::addGate(GateType type)
{
    CAST_SOUND;
    const ConnectionID id = ConnectionID(bit_sound->components.getNumChildren()) + 1; // FUTURE May need to be handled differently for multi-output sources

    bit_sound->components.appendChild(newGateRep(id, type), bit_sound->undo_manager);
    const auto gate = getGate(id);

    for(const auto& voice : voices)
    {
        CAST_VOICE(voice);
        bit_voice->gates.emplace_back(selectNewGate(gate));
    }
    return id;
}

void BitSynthesizer::setGateInput(ConnectionID id, ConnectionID input_id, size_t input_index)
{
    auto gate = getGate(id);
    jassert(input_index < gate[name::CONNECTIONS].getArray()->size());
    // FUTURE FIXME wouldn't be undone by the undo manager if implemented!
    // Option 1 - recreate the array
    // Option 2 - use subtrees - though it wouldn't be xml-compatible then
    connections(gate)[input_index] = input_id;
    auto source = getSource(input_id);
    const ConnectionID source_id = source[name::ID];
    for(auto& voice : voices)
    {
        auto* bit_voice = castVoice(voice);

        BitSource* bit_source = nullptr;
        if(source_id > 0)
            bit_source = bit_voice->gates[ConnectionID(source[name::INDEX])].get();
        else if(source_id < 0)
            bit_source = bit_voice->oscillators[ConnectionID(source[name::INDEX])].get();

        bit_voice->gates[ConnectionID(gate[name::INDEX])]->setInput(bit_source, input_index);
    }
}

ConnectionID BitSynthesizer::addMixChannel()
{
    CAST_SOUND;
    const ConnectionID id = ConnectionID(bit_sound->sinks.getNumChildren()) + 1;

    bit_sound->sinks.appendChild(newBitMixChannelRep(id), bit_sound->undo_manager);

    for(const auto& voice : voices)
    {
        CAST_VOICE(voice);
        bit_voice->bit_inputs.emplace_back(new BitMixChannel());
    }
    return id;
}

void BitSynthesizer::setMixChannelInput(ConnectionID id, const ConnectionID input_id)
{
    CAST_SOUND;
    auto mix = getMixChannel(id);
    mix.setProperty(name::CONNECTIONS, input_id, bit_sound->undo_manager);
    auto source = getSource(mix[name::CONNECTIONS]);
    const ConnectionID source_id = source[name::ID];
    for(auto& voice : voices)
    {
        auto* bit_voice = castVoice(voice);

        BitSource* bit_source = nullptr;
        if(source_id > 0)
            bit_source = bit_voice->gates[ConnectionID(source[name::INDEX])].get();
        else if(source_id < 0)
            bit_source = bit_voice->oscillators[ConnectionID(source[name::INDEX])].get();

        bit_voice->bit_inputs[ConnectionID(mix[name::INDEX])]->setInput(bit_source);
    }

}

void BitSynthesizer::setMixChannelLevel(ConnectionID id, float level)
{
    CAST_SOUND;
    auto mix = getMixChannel(id);
    mix.setProperty(name::LEVEL, level, bit_sound->undo_manager);

    for(auto& voice : voices)
    {
        auto* bit_voice = castVoice(voice);

        bit_voice->bit_inputs[ConnectionID(mix[name::INDEX])]->setLevel(mix[name::LEVEL]);
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






