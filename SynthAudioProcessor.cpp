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
    // Perhaps should be moved to the state manager?
    // Add subtrees for the dynamic components of the synth. Warning - never add an undo manager here!
    parameters.state.appendChild(juce::ValueTree(name::GENERATORS), nullptr);
    parameters.state.appendChild(juce::ValueTree(name::COMPONENTS), nullptr);
    parameters.state.appendChild(juce::ValueTree(name::SINKS), nullptr);

}

void SynthAudioProcessor::getStateInformation(juce::MemoryBlock& dest_data)
{
    const auto state = parameters.copyState();
    const auto xml(state.createXml());
    copyXmlToBinary(*xml, dest_data);
}

void SynthAudioProcessor::setStateInformation(const void* data, const int size_in_bytes)
{
    const auto xml_element(getXmlFromBinary(data, size_in_bytes));
    if(xml_element.get() == nullptr || !xml_element->hasTagName(parameters.state.getType()))
        return;
    parameters.replaceState(juce::ValueTree::fromXml(*xml_element));
}

void SynthAudioProcessor::prepareToPlay(double, int)
{
}

void SynthAudioProcessor::releaseResources()
{
}

void SynthAudioProcessor::processBlock(juce::AudioSampleBuffer& buffer, juce::MidiBuffer&)
{
}

