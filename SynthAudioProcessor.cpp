//
// Created by micha on 12.04.2026.
//

#include "SynthAudioProcessor.h"


#include "synth_management/SynthManagementNames.h"




SynthAudioProcessor::SynthAudioProcessor()
    : parameters(
        *this, nullptr, "BitSynth", SynthStateManager::createParameterLayout()
    ),
    state_manager(parameters, state_manager_state)
{
    initState();
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
    if(xml_element.get() == nullptr || !xml_element->hasTagName(parameters.state.getType()))
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

void SynthAudioProcessor::prepareToPlay(double, int)
{
    // TODO - add prepareToPlay() method to the BitSynthesizer
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
            {name::META_NEXT_FREE_DYNAMIC_PARAMETER_ID, 0}
        }), nullptr);

    bit_synth = std::make_unique<BitSynthesizer>(num_voices, state_manager);
    parameters.state.getChildWithName(name::GENERATORS).addListener(bit_synth.get());
    parameters.state.getChildWithName(name::COMPONENTS).addListener(bit_synth.get());
    parameters.state.getChildWithName(name::SINKS).addListener(bit_synth.get());
}

