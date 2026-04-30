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
    return *tree[Name::CONNECTIONS].getArray();
}


BitSynthesizer::BitSynthesizer(int num_voices, const SynthStateManager& state_manager)
    : state_manager(state_manager)
{
    addSound(new BitSynthSound());
    for(int i = 0; i < num_voices; i++)
        addVoice(new BitSynthVoice());
}

void BitSynthesizer::addOscillator(const juce::ValueTree& tree)
{
    throwassert(tree.isValid(),
                InvalidTreeError("Invalid oscillator ValueTree"));
    throwassert(tree.getType() == Name::OSCILLATOR,
                InvalidTreeError("Invalid type for oscillator"));
    throwassert(tree.hasProperty(Name::ID),
                InvalidTreeError("No valid ID in oscillator ValueTree"));

#ifdef DEBUG_VERBOSE
    juce::XmlElement::TextFormat format;
#ifndef _WIN32
    format.newLineChars = "\n";
#endif
    std::cout << tree.toXmlString(format) << std::endl;
    std::cout << state_manager.parameters.getParameter(tree[Name::RATIO].toString()) << std::endl;
#endif

    auto* const ratio_param = state_manager.parameters.getRawParameterValue(tree[Name::RATIO].toString());
    auto* const pw_param = state_manager.parameters.getRawParameterValue(tree[Name::PULSE_WIDTH].toString());
    auto* const starting_phase_param = state_manager.parameters.getRawParameterValue(tree[Name::STARTING_PHASE].toString());
    throwassert(pw_param != nullptr && ratio_param != nullptr && starting_phase_param != nullptr,
                InvalidTreeError("Invalid dynamic parameter ID in oscillator"));

    for(const auto& voice : voices)
    {
        BIT_VOICE(voice);
        bit_voice->oscillators.emplace_back(
            new Oscillator(*ratio_param, *pw_param, *starting_phase_param)
        );
    }
}

inline ptr<GateNode> selectNewGate(const juce::ValueTree& gate)
{
    const auto type = gate.getType();
    if(type == Name::GATE_NOT) return ptr<GateNode>(new NotGate());
    if(type == Name::GATE_AND) return ptr<GateNode>(new AndGate());
    if(type == Name::GATE_OR)  return ptr<GateNode>(new OrGate());
    if(type == Name::GATE_XOR) return ptr<GateNode>(new XorGate());
    jassertfalse; return nullptr;
}

void BitSynthesizer::addGate(const juce::ValueTree& gate)
{
    throwassert(gate.isValid(),
                InvalidTreeError("Invalid gate ValueTree"));
    throwassert(isIdentifierAGate(gate.getType()),
                InvalidTreeError("Invalid type for gate"));
    throwassert(gate.hasProperty(Name::ID),
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
        else if(matchesSign(source_id, SIGN_PROCESSOR))
        {
            jassert(source_id != 0);
            bit_source = bit_voice->gates[getProcessorIndex(source_element_id)].get();
        }
        else
        {
            jassert(source_id == 0); // if == 0, it's supposed to stay nullptr
        }

        if(matchesSign(id, SIGN_PROCESSOR))
            bit_voice->gates[getProcessorIndex(id)]->setInput(bit_source, sub_connection_id);
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
    for(const auto& connection : element.getChildWithName(Name::CONNECTIONS))
    {
        throwassert(connection.hasProperty(Name::ID),
                    InvalidTreeError("Invalid connection ID in connection ValueTree"));
        const ConnectionID source_id = connection[Name::ID];
        const SubConnectionID target_sub_id = element.getChildWithName(Name::CONNECTIONS).indexOf(connection); // Kinda dumb
        // Could get rid of the above, if I added the target ConnectionID to the Connection node... Hmmm...

        const ElementID element_id = element[Name::ID];
        throwassert(matchesSign(element_id, SIGN_PROCESSOR) && isIdentifierAGate(element.getType())
                || matchesSign(element_id, SIGN_SINK && isIdentifierASink(element.getType())),
                InvalidTreeError("Invalid source or ID for connection"));
        setInput(element_id, source_id, target_sub_id);
    }
}

void BitSynthesizer::addMixChannel(const juce::ValueTree& mix)
{
    throwassert(mix.getType() == Name::MIX_CHANNEL,
                InvalidTreeError("Invalid type for mix channel"));
    throwassert(mix.hasProperty(Name::ID),
                InvalidTreeError("No valid ID in mix channel"));
    throwassert(mix.getChildWithName(Name::CONNECTIONS).isValid(),
                InvalidTreeError("No valid connection in mix channel"));
    throwassert(mix.hasProperty(Name::LEVEL),
                InvalidTreeError("No level property in mix channel"));

    auto* const level_param = state_manager.parameters.getRawParameterValue(mix[Name::LEVEL].toString());
    throwassert(level_param != nullptr,
                InvalidTreeError("Invalid dynamic parameter ID in mix channel"));

    for(const auto& voice : voices)
    {
        BIT_VOICE(voice);
        bit_voice->bit_inputs.emplace_back(new BitMixChannel(*level_param));
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

    const auto generators = root.getChildWithName(Name::GENERATORS);
    const auto processors = root.getChildWithName(Name::PROCESSORS);
    const auto sinks = root.getChildWithName(Name::SINKS);
    throwassert(generators.isValid() && processors.isValid() && sinks.isValid(),
                InvalidTreeError("Root tree must have generators, components and sinks subtrees"));

    // Create the components
    for(const auto& generator : generators)
    {
        throwassert(generator.getType() == Name::OSCILLATOR,
                    InvalidTreeError("Invalid child type in generators tree"));
        addOscillator(generator);
    }
    for(const auto& processor : processors)
    {
        throwassert(isIdentifierAGate(processor.getType()),
                    InvalidTreeError("Invalid child type in components tree"));
        addGate(processor);
    }
    for(const auto& sink : sinks)
    {
        throwassert(isIdentifierASink(sink.getType()),
                    InvalidTreeError("Invalid child type in sinks tree"));
        addMixChannel(sink);
    }

    // Reestabilish connections
    for(const auto& processor : processors)
    {
        setInputs(processor);
    }
    for(const auto& sink : sinks)
    {
        setInputs(sink);
    }

    // I believe the synthesizer should be in a valid state now.
}


void BitSynthesizer::valueTreePropertyChanged(juce::ValueTree& affected_tree, const juce::Identifier& property)
{
    if(affected_tree.getType() == Name::CONNECTION)
    {
        throwassert(property == Name::ID && affected_tree[Name::ID].isInt64(),
                    InvalidTreeError("Invalid connection ID in connection ValueTree"));
        const ConnectionID source_id = affected_tree[Name::ID];
        const SubConnectionID target_sub_id = affected_tree.getParent().indexOf(affected_tree); // Kinda dumb
        // Backtracking from element/Connections/Connection
        const juce::ValueTree element_tree = affected_tree.getParent().getParent();
        throwassert(element_tree.isValid(),
                    InvalidTreeError("No valid parent of set connection"));
        // Could get rid of the above, if I added the target ConnectionID to the Connection node... Hmmm...

        const ElementID element_id = element_tree[Name::ID];
        throwassert(matchesSign(element_id, SIGN_PROCESSOR) && isIdentifierAGate(element_tree.getType())
                || matchesSign(element_id, SIGN_SINK && isIdentifierASink(element_tree.getType())),
                InvalidTreeError("Invalid source or ID for connection"));

        setInput(element_id, source_id, target_sub_id);
    }
}

void BitSynthesizer::valueTreeChildAdded(juce::ValueTree& parent_tree, juce::ValueTree& child_tree)
{
    if(parent_tree.getType() == Name::GENERATORS)
    {
        if(child_tree.getType() == Name::OSCILLATOR)
        {
            addOscillator(child_tree);
        }
        else throw InvalidTreeError("Invalid child type added to generators tree");
    }
    else if(parent_tree.getType() == Name::PROCESSORS)
    {
        if(isIdentifierAGate(child_tree.getType()))
        {
            addGate(child_tree);
        }
        else throw InvalidTreeError("Invalid child type added to components tree");
    }
    else if(parent_tree.getType() == Name::SINKS)
    {
        if(child_tree.getType() == Name::MIX_CHANNEL)
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








