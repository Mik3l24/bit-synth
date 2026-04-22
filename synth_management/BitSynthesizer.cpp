//
// Created by micha on 15.11.2023.
//

#include "BitSynthesizer.h"

#include "dsp/BitSynthVoice.h"
#include "BitSynthSound.h"
#include "SynthManagementNames.h"
#include "Errors.h"

inline BitSynthVoice* castVoice(juce::SynthesiserVoice* voice)
{
    auto* bit_voice = dynamic_cast<BitSynthVoice*>(voice); // Should be always correct, as only BitSynthVoices are added to the synthesizer
    jassert(bit_voice != nullptr);
    return bit_voice;
}

inline BitSynthSound* castSound(juce::SynthesiserSound* sound)
{
    auto* bit_sound = dynamic_cast<BitSynthSound*>(sound); // Should be always correct, as only BitSynthSounds are added to the synthesizer
    jassert(bit_sound != nullptr);
    return bit_sound;
}

#define BIT_VOICE(voice) auto* bit_voice = castVoice(voice)
#define CAST_SOUND jassert(sounds.size() == 1); auto* bit_sound = castSound(sounds.getFirst())


inline juce::Array<juce::var> connections(const juce::ValueTree& tree)
{
    return *tree[name::CONNECTIONS].getArray();
}


BitSynthesizer::BitSynthesizer(int num_voices, const SynthStateManager& state_manager)
    : state_manager(state_manager)
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


void BitSynthesizer::addOscillator(const juce::ValueTree& tree)
{
    throwassert(tree.isValid(),
                InvalidTreeError("Invalid oscillator ValueTree"));
    throwassert(tree.getType() == name::OSCILLATOR,
                InvalidTreeError("Invalid type for oscillator"));
    throwassert(tree.hasProperty(name::ID) && tree[name::ID].isInt64(),
                InvalidTreeError("No valid ID in oscillator ValueTree"));

    for(const auto& voice : voices)
    {
        BIT_VOICE(voice);
        bit_voice->oscillators.emplace_back(new Oscillator());
        // TODO connect parameters
    }
}

inline ptr<GateNode> selectNewGate(const juce::ValueTree& gate)
{
    const auto type = gate.getType();
    if(type == name::GATE_NOT) return ptr<GateNode>(new NotGate());
    if(type == name::GATE_AND) return ptr<GateNode>(new AndGate());
    if(type == name::GATE_OR)  return ptr<GateNode>(new OrGate());
    if(type == name::GATE_XOR) return ptr<GateNode>(new XorGate());
    jassertfalse; return nullptr;
}

void BitSynthesizer::addGate(const juce::ValueTree& gate)
{
    throwassert(gate.isValid(),
                InvalidTreeError("Invalid gate ValueTree"));
    throwassert(isIdentifierAGate(gate.getType()),
                InvalidTreeError("Invalid type for gate"));
    throwassert(gate.hasProperty(name::ID) && gate[name::ID].isInt64(),
                InvalidTreeError("No valid ID in gate ValueTree"));

    for(const auto& voice : voices)
    {
        BIT_VOICE(voice);
        bit_voice->gates.emplace_back(selectNewGate(gate));
    }
}

void BitSynthesizer::setInput(ElementID id, ConnectionID source_id, SubConnectionID sub_connection_id)
{

    const auto [source_element_id, source_subconnection_id, source_sign] = decodeConnectionID(source_id);

    for(auto& voice : voices)
    {
        BIT_VOICE(voice);

        // FUTURE - Will need refactoring if multi-output elements are introduced
        BitSource* bit_source = nullptr;
        if(matchesSign(source_id, SIGN_GENERATOR))
        {
            jassert(source_id != 0);
            bit_source = bit_voice->oscillators[getGeneratorIndex(source_element_id)].get();
        }
        else if(matchesSign(source_id, SIGN_COMPONENT))
        {
            jassert(source_id != 0);
            bit_source = bit_voice->gates[getComponentIndex(source_element_id)].get();
        }
        else
        {
            jassert(source_id == 0); // if == 0, it's supposed to stay nullptr
        }

        if(matchesSign(id, SIGN_COMPONENT))
            bit_voice->gates[getComponentIndex(id)]->setInput(bit_source, sub_connection_id);
        else if(matchesSign(id, SIGN_SINK))
            bit_voice->bit_inputs[getSinkIndex(id)]->setInput(bit_source);
        else
            jassertfalse;
    }
}

void BitSynthesizer::setInputs(const juce::ValueTree& element)
{
    throwassert(isIdentifierAGate(element.getType()) || isIdentifierASink(element.getType()),
                InvalidTreeError("Element must be a gate or a sink to set inputs"));
    for(const auto& connection : element.getChildWithName(name::CONNECTIONS))
    {
        throwassert(connection.hasProperty(name::ID) && connection[name::ID].isInt64(),
                    InvalidTreeError("Invalid connection ID in connection ValueTree"));
        const ConnectionID source_id = connection[name::ID];
        const SubConnectionID target_sub_id = element.getChildWithName(name::CONNECTIONS).indexOf(connection); // Kinda dumb
        // Could get rid of the above, if I added the target ConnectionID to the Connection node... Hmmm...

        const ElementID element_id = element[name::ID];
        throwassert(matchesSign(element_id, SIGN_COMPONENT) && isIdentifierAGate(element.getType())
                || matchesSign(element_id, SIGN_SINK && isIdentifierASink(element.getType())),
                InvalidTreeError("Invalid source or ID for connection"));
        setInput(element_id, source_id, target_sub_id);
    }
}

void BitSynthesizer::addMixChannel(const juce::ValueTree& mix)
{
    throwassert(mix.getType() == name::MIX_CHANNEL,
                InvalidTreeError("Invalid type for mix channel"));
    throwassert(mix.hasProperty(name::ID) && mix[name::ID].isInt64(),
                InvalidTreeError("No valid ID in mix channel"));
    throwassert(mix.hasProperty(name::CONNECTIONS) && mix[name::CONNECTIONS].isInt64(),
                InvalidTreeError("No valid connection in mix channel"));
    throwassert(mix.hasProperty(name::LEVEL),
                InvalidTreeError("No level property in mix channel"));

    for(const auto& voice : voices)
    {
        BIT_VOICE(voice);
        bit_voice->bit_inputs.emplace_back(new BitMixChannel());
    }
}

void BitSynthesizer::reconstructSynthFromTree(juce::ValueTree& root)
{
    // Clear current voices
    for(const auto& voice : voices)
    {
        BIT_VOICE(voice);
        bit_voice->oscillators.clear();
        bit_voice->gates.clear();
        bit_voice->bit_inputs.clear();
    }

    const auto generators = root.getChildWithName(name::GENERATORS);
    const auto components = root.getChildWithName(name::COMPONENTS);
    const auto sinks = root.getChildWithName(name::SINKS);
    throwassert(generators.isValid() && components.isValid() && sinks.isValid(),
                InvalidTreeError("Root tree must have generators, components and sinks subtrees"));

    // Create the components
    for(const auto& generator : generators)
    {
        throwassert(generator.getType() == name::OSCILLATOR,
                    InvalidTreeError("Invalid child type in generators tree"));
        addOscillator(generator);
    }
    for(const auto& component : components)
    {
        throwassert(isIdentifierAGate(component.getType()),
                    InvalidTreeError("Invalid child type in components tree"));
        addGate(component);
    }
    for(const auto& sink : sinks)
    {
        throwassert(isIdentifierASink(sink.getType()),
                    InvalidTreeError("Invalid child type in sinks tree"));
        addMixChannel(sink);
    }

    // Reestabilish connections
    for(const auto& component : components)
    {
        setInputs(component);
    }
    for(const auto& sink : sinks)
    {
        setInputs(sink);
    }

    // I believe the synthesizer should be in a valid state now.
}


void BitSynthesizer::valueTreePropertyChanged(juce::ValueTree& affected_tree, const juce::Identifier& property)
{
    if(affected_tree.getType() == name::CONNECTION)
    {
        throwassert(property == name::ID && affected_tree[name::ID].isInt64(),
                    InvalidTreeError("Invalid connection ID in connection ValueTree"));
        const ConnectionID source_id = affected_tree[name::ID];
        const SubConnectionID target_sub_id = affected_tree.getParent().indexOf(affected_tree); // Kinda dumb
        // Backtracking from element/Connections/Connection
        const juce::ValueTree element_tree = affected_tree.getParent().getParent();
        throwassert(element_tree.isValid(),
                    InvalidTreeError("No valid parent of set connection"));
        // Could get rid of the above, if I added the target ConnectionID to the Connection node... Hmmm...

        const ElementID element_id = element_tree[name::ID];
        throwassert(matchesSign(element_id, SIGN_COMPONENT) && isIdentifierAGate(element_tree.getType())
                || matchesSign(element_id, SIGN_SINK && isIdentifierASink(element_tree.getType())),
                InvalidTreeError("Invalid source or ID for connection"));

        setInput(element_id, source_id, target_sub_id);
    }
}

void BitSynthesizer::valueTreeChildAdded(juce::ValueTree& parent_tree, juce::ValueTree& child_tree)
{
    if(parent_tree.getType() == name::GENERATORS)
    {
        if(child_tree.getType() == name::OSCILLATOR)
        {
            addOscillator(child_tree);
        }
        else throw InvalidTreeError("Invalid child type added to generators tree");
    }
    else if(parent_tree.getType() == name::COMPONENTS)
    {
        if(isIdentifierAGate(child_tree.getType()))
        {
            addGate(child_tree);
        }
        else throw InvalidTreeError("Invalid child type added to components tree");
    }
    else if(parent_tree.getType() == name::SINKS)
    {
        if(child_tree.getType() == name::MIX_CHANNEL)
        {
            addMixChannel(child_tree);
        }
        else throw InvalidTreeError("Invalid child type added to sinks tree");
    }
    // We don't care about any other additions I guess
}

void BitSynthesizer::valueTreeChildRemoved(juce::ValueTree& parent_tree, juce::ValueTree& child_tree, int removed_child_i)
{
    jassertfalse; // Not implemented yet, but will be needed for element removal
}

void BitSynthesizer::valueTreeRedirected(juce::ValueTree& affected_tree)
{

}








