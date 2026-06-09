//
// Created by micha on 15.11.2023.
//

#include "BitSynthesizer.h"

#include "dsp/BitSynthVoice.h"
#include "BitSynthSound.h"
#include "SynthManagementNames.h"
#include "Errors.h"

inline dsp::BitSynthVoice* castVoice(juce::SynthesiserVoice* voice)
{
    auto* bit_voice = dynamic_cast<dsp::BitSynthVoice*>(voice); // Should be always correct, as only BitSynthVoices are added to the synthesizer
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
    return *tree[Name::DEPENDENCIES].getArray();
}


BitSynthesizer::BitSynthesizer(int num_voices, const SynthStateManager& _state_manager)
    : state_manager(_state_manager)
{
    addSound(new BitSynthSound());
    for(int i = 0; i < num_voices; i++)
        addVoice(new dsp::BitSynthVoice());
}

void BitSynthesizer::prepareToPlay(double _sample_rate, int _samples_per_block)
{
    sample_rate = _sample_rate;
    samples_per_block = _samples_per_block;
    zeroes.resize(size_t(samples_per_block), 0);
    setCurrentPlaybackSampleRate(sample_rate);
    prepareToPlayVoices();
}

void BitSynthesizer::prepareToPlayVoices()
{
    for(const auto& voice : voices)
    {
        BIT_VOICE(voice);
        bit_voice->prepareToPlay(sample_rate, samples_per_block);
    }
}

void BitSynthesizer::appendOscillator(const juce::ValueTree& tree)
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
            new dsp::Oscillator(*ratio_param, *pw_param, *starting_phase_param)
        )->prepareToPlay(sample_rate, samples_per_block);
    }
}

dsp::ptr<dsp::Gate> BitSynthesizer::selectNewGate(const juce::ValueTree& gate)
{
    const auto type = gate.getType();
    if(type == Name::GATE_NOT) return dsp::ptr<dsp::Gate>(new dsp::NotGate(zeroes));
    if(type == Name::GATE_AND) return dsp::ptr<dsp::Gate>(new dsp::AndGate(zeroes));
    if(type == Name::GATE_OR)  return dsp::ptr<dsp::Gate>(new dsp::OrGate(zeroes));
    if(type == Name::GATE_XOR) return dsp::ptr<dsp::Gate>(new dsp::XorGate(zeroes));
    jassertfalse; return nullptr;
}

void BitSynthesizer::appendGate(const juce::ValueTree& gate)
{
    throwassert(gate.isValid(),
                InvalidTreeError("Invalid gate ValueTree"));
    throwassert(isIdentifierAGate(gate.getType()),
                InvalidTreeError("Invalid type for gate"));
    throwassert(gate.hasProperty(Name::ID),
                InvalidTreeError("No valid ID in gate ValueTree"));
    throwassert(gate.hasProperty(Name::INDEX),
                InvalidTreeError("No index in gate ValueTree"));

    const ElementID id = gate[Name::ID];
    const ElementOrder index = gate[Name::INDEX];
    jassert(index == id - 1);
    processor_order.push_back(index);

    for(const auto& voice : voices)
    {
        BIT_VOICE(voice);
        bit_voice->gates.emplace_back(selectNewGate(gate))
            ->prepareToPlay(sample_rate, samples_per_block);
    }
}

void BitSynthesizer::placeGate(const juce::ValueTree& gate)
{
    throwassert(gate.isValid(),
                InvalidTreeError("Invalid gate ValueTree"));
    throwassert(isIdentifierAGate(gate.getType()),
                InvalidTreeError("Invalid type for gate"));
    throwassert(gate.hasProperty(Name::ID),
                InvalidTreeError("No valid ID in gate ValueTree"));
    throwassert(gate.hasProperty(Name::INDEX),
                InvalidTreeError("No index in gate ValueTree"));

    const ElementID id = gate[Name::ID];
    const ElementOrder index = gate[Name::INDEX];
    jassert(size_t(id - 1) < processor_order.size());
    processor_order[size_t(id - 1)] = index;

    for(const auto& voice : voices)
    {
        BIT_VOICE(voice);
        jassert(size_t(index) < bit_voice->gates.size());
        // ReSharper disable once CppTemplateArgumentsCanBeDeduced // Template has to be specified explicitly for MSVC
        (bit_voice->gates[index] = dsp::ptr<dsp::Gate>(selectNewGate(gate)))
            ->prepareToPlay(sample_rate, samples_per_block);
    }
}

void BitSynthesizer::setInput(ElementID id, ConnectionID source_id, SubConnectionID sub_connection_id)
{

    const auto [source_element_id, source_subconnection_id, source_sign] = decodeConnectionID(source_id);

    for(auto& voice : voices)
    {
        BIT_VOICE(voice);

        // FUTURE - Will need refactoring if multi-output elements are introduced
        dsp::BitSource* bit_source = nullptr;
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
    for(const auto& connection : element.getChildWithName(Name::DEPENDENCIES))
    {
        throwassert(connection.hasProperty(Name::ID),
                    InvalidTreeError("Invalid connection ID in connection ValueTree"));
        const ConnectionID source_id = connection[Name::ID];
        const SubConnectionID target_sub_id = element.getChildWithName(Name::DEPENDENCIES).indexOf(connection); // Kinda dumb
        // Could get rid of the above, if I added the target ConnectionID to the Connection node... Hmmm...

        const ElementID element_id = element[Name::ID];
        throwassert(matchesSign(element_id, SIGN_PROCESSOR) && isIdentifierAGate(element.getType())
                || matchesSign(element_id, SIGN_SINK && isIdentifierASink(element.getType())),
                InvalidTreeError("Invalid source or ID for connection"));
        setInput(element_id, source_id, target_sub_id);
    }
}

void BitSynthesizer::replaceProcessors(const std::vector<juce::ValueTree>& processors_to_replace)
{
    for(const auto& processor : processors_to_replace)
    {
        jassert(processor.isValid());
        jassert(isIdentifierAGate(processor.getType())); // For now, until other types are introduced
        placeGate(processor);
    }
}

void BitSynthesizer::appendMixChannel(const juce::ValueTree& mix)
{
    throwassert(mix.getType() == Name::MIX_CHANNEL,
                InvalidTreeError("Invalid type for mix channel"));
    throwassert(mix.hasProperty(Name::ID),
                InvalidTreeError("No valid ID in mix channel"));
    throwassert(mix.getChildWithName(Name::DEPENDENCIES).isValid(),
                InvalidTreeError("No valid connection in mix channel"));
    throwassert(mix.hasProperty(Name::LEVEL),
                InvalidTreeError("No level property in mix channel"));

    auto* const level_param = state_manager.parameters.getRawParameterValue(mix[Name::LEVEL].toString());
    throwassert(level_param != nullptr,
                InvalidTreeError("Invalid dynamic parameter ID in mix channel"));

    for(const auto& voice : voices)
    {
        BIT_VOICE(voice);
        bit_voice->bit_inputs.emplace_back(new dsp::BitMixChannel(zeroes, *level_param))
            ->prepareToPlay(sample_rate, samples_per_block);
    }
}

void BitSynthesizer::reconstructSynthFromTree(const juce::ValueTree& root)
{
    const auto generators = root.getChildWithName(Name::GENERATORS);
    const auto processors = root.getChildWithName(Name::PROCESSORS);
    const auto sinks = root.getChildWithName(Name::SINKS);
    throwassert(generators.isValid() && processors.isValid() && sinks.isValid(),
                InvalidTreeError("Root tree must have generators, components and sinks subtrees"));

    // Clear current voices
    processor_order.clear();
    processor_order.resize(size_t(processors.getNumChildren()));
    for(const auto& voice : voices)
    {
        BIT_VOICE(voice);
        bit_voice->oscillators.clear();
        bit_voice->gates.clear();
        bit_voice->gates.resize(size_t(processors.getNumChildren()));
        bit_voice->bit_inputs.clear();
    }

    // Create the components
    for(const auto& generator : generators)
    {
        throwassert(generator.getType() == Name::OSCILLATOR,
                    InvalidTreeError("Invalid child type in generators tree"));
        appendOscillator(generator);
    }
    for(const auto& processor : processors)
    {
        throwassert(isIdentifierAGate(processor.getType()),
                    InvalidTreeError("Invalid child type in components tree"));
        placeGate(processor);
    }
    for(const auto& sink : sinks)
    {
        throwassert(isIdentifierASink(sink.getType()),
                    InvalidTreeError("Invalid child type in sinks tree"));
        appendMixChannel(sink);
    }

    reconstructConnectionsFromTree(root);

    // I believe the synthesizer should be in a valid state now.
}

void BitSynthesizer::reconstructConnectionsFromTree(const juce::ValueTree& root)
{
    const auto generators = root.getChildWithName(Name::GENERATORS);
    const auto processors = root.getChildWithName(Name::PROCESSORS);
    const auto sinks = root.getChildWithName(Name::SINKS);
    throwassert(generators.isValid() && processors.isValid() && sinks.isValid(),
                InvalidTreeError("Root tree must have generators, components and sinks subtrees"));

    // Reestabilish connections
    for(const auto& processor : processors)
    {
        setInputs(processor);
    }
    for(const auto& sink : sinks)
    {
        setInputs(sink);
    }
}


void BitSynthesizer::valueTreePropertyChanged(juce::ValueTree& affected_tree, const juce::Identifier& property)
{
    if(affected_tree.getType() == Name::DEPENDENCY)
    {
        if(state_manager.meta.temp.sorting_affected_processors.empty())
        { // Ordering did not change, we can do a simple input setting
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
        else
        { // We need to change the order
            replaceProcessors(state_manager.meta.temp.sorting_affected_processors);
            state_manager.meta.temp.sorting_affected_processors.clear();
            // Elements that weren't in the affected processors might depend on those that were moved,
            // so we need to remake ALL connections just in case
            reconstructConnectionsFromTree(state_manager.parameters.state);
        }
        // debug print
#ifdef DEBUG_VERBOSE
        std::cout << "Processor order: \n\t";
        for(const auto ord : processor_order) std::cout << ord << "\t";
        std::cout << std::endl;
        std::cout << "@\t";
        for(size_t i = 0; i < processor_order.size(); i++) std::cout << i << "\t";
        std::cout << std::endl;
#endif
    }
}

void BitSynthesizer::valueTreeChildAdded(juce::ValueTree& parent_tree, juce::ValueTree& child_tree)
{
    if(parent_tree.getType() == Name::GENERATORS)
    {
        if(child_tree.getType() == Name::OSCILLATOR)
        {
            appendOscillator(child_tree);
        }
        else throw InvalidTreeError("Invalid child type added to generators tree");
    }
    else if(parent_tree.getType() == Name::PROCESSORS)
    {
        if(isIdentifierAGate(child_tree.getType()))
        {
            appendGate(child_tree);
        }
        else throw InvalidTreeError("Invalid child type added to components tree");
    }
    else if(parent_tree.getType() == Name::SINKS)
    {
        if(child_tree.getType() == Name::MIX_CHANNEL)
        {
            appendMixChannel(child_tree);
        }
        else throw InvalidTreeError("Invalid child type added to sinks tree");
    }
    // We don't care about any other additions I guess
}

void BitSynthesizer::valueTreeChildRemoved(juce::ValueTree& parent_tree, juce::ValueTree& child_tree, int removed_child_i)
{
    juce::ignoreUnused(parent_tree, child_tree, removed_child_i);
    // Not implemented yet, but will be needed for element removal
}

void BitSynthesizer::valueTreeRedirected(juce::ValueTree& affected_tree)
{
    juce::ignoreUnused(affected_tree);
}








