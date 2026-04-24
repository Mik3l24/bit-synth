//
// Created by micha on 12.04.2026.
//

#include "SynthAudioProcessor.h"


#include "synth_management/SynthManagementNames.h"




SynthAudioProcessor::SynthAudioProcessor()
    : parameters(
        *this, nullptr, "BitSynth", SynthStateManager::createParameterLayout()
    ),
    state_manager(parameters, internal_state)
{
    initState();
}

SynthAudioProcessor::~SynthAudioProcessor()
{
    bit_synth = nullptr;
}

void SynthAudioProcessor::getStateInformation(juce::MemoryBlock& dest_data)
{
    auto state = parameters.copyState();
    const auto xml(state.createXml());
    copyXmlToBinary(*xml, dest_data);
}

void SynthAudioProcessor::setStateInformation(const void* data, const int size_in_bytes)
{
    const auto xml_element(getXmlFromBinary(data, size_in_bytes));
    if(xml_element == nullptr || !xml_element->hasTagName(parameters.state.getType()))
        return;
    parameters.replaceState(juce::ValueTree::fromXml(*xml_element));
    initState();

#ifdef DEBUG_VERBOSE
    juce::XmlElement::TextFormat format;
#ifndef _WIN32
    format.newLineChars = "\n";
#endif
    std::cout << "Generators:\n" << parameters.state.getChildWithName(name::GENERATORS).toXmlString(format) << std::endl;
    std::cout << "Components:\n" << parameters.state.getChildWithName(name::COMPONENTS).toXmlString(format) << std::endl;
    std::cout << "Sinks:\n" << parameters.state.getChildWithName(name::SINKS).toXmlString(format) << std::endl;
#endif

    bit_synth->reconstructSynthFromTree(parameters.state);
    // TODO - UI update (though, perhaps valueTreeRedirected() is what we need)
}

void SynthAudioProcessor::prepareToPlay(const double sample_rate, const int max_samples_per_block)
{
    // TODO - add prepareToPlay() method to the BitSynthesizer
    bit_synth->setCurrentPlaybackSampleRate(sample_rate);
}

void SynthAudioProcessor::releaseResources()
{
    // TODO - add releaseResources() method to the BitSynthesizer
}

void SynthAudioProcessor::processBlock(juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midi)
{
    bit_synth->renderNextBlock(buffer, midi, 0, buffer.getNumSamples());
}

void SynthAudioProcessor::initState()
{
    // Perhaps should be moved to the state manager?
    // Add subtrees for the dynamic components of the synth. Warning - never add an undo manager here!
    if(!parameters.state.getChildWithName(name::GENERATORS).isValid())
        parameters.state.appendChild(juce::ValueTree(name::GENERATORS), nullptr);
    if(!parameters.state.getChildWithName(name::COMPONENTS).isValid())
        parameters.state.appendChild(juce::ValueTree(name::COMPONENTS), nullptr);
    if(!parameters.state.getChildWithName(name::SINKS).isValid())
        parameters.state.appendChild(juce::ValueTree(name::SINKS), nullptr);
    if(!parameters.state.getChildWithName(name::META_STATE).isValid())
        parameters.state.appendChild(juce::ValueTree(name::META_STATE, {
            {name::META_NEXT_FREE_DYNAMIC_PARAMETER_ID, 0} // FIXME low priority - if there exist trees with parameters already, then this would be an invalid value. However - that would already be an incorrect state anyway.
        }), nullptr);

    internal_state.generators = parameters.state.getChildWithName(name::GENERATORS);
    jassert(internal_state.generators.isValid());
    internal_state.components = parameters.state.getChildWithName(name::COMPONENTS);
    jassert(internal_state.components.isValid());
    internal_state.sinks = parameters.state.getChildWithName(name::SINKS);
    jassert(internal_state.sinks.isValid());

    bit_synth = std::make_unique<BitSynthesizer>(num_voices, state_manager);
    internal_state.generators.addListener(bit_synth.get());
    internal_state.components.addListener(bit_synth.get());
    internal_state.sinks.addListener(bit_synth.get());
}

