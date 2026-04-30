//
// Created by micha on 12.04.2026.
//

#include "SynthAudioProcessor.h"


#include "synth_management/SynthManagementNames.h"




SynthAudioProcessor::SynthAudioProcessor()
    : parameters(
        *this, nullptr, Name::ROOT, SynthStateManager::createParameterLayout()
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
    std::cout << "Generators:\n" << parameters.state.getChildWithName(Name::GENERATORS).toXmlString(format) << std::endl;
    std::cout << "Components:\n" << parameters.state.getChildWithName(Name::PROCESSORS).toXmlString(format) << std::endl;
    std::cout << "Sinks:\n" << parameters.state.getChildWithName(Name::SINKS).toXmlString(format) << std::endl;
#endif

    bit_synth->reconstructSynthFromTree(parameters.state);
    if(change_listener != nullptr)
        change_listener->stateReplaced();
}

void SynthAudioProcessor::prepareToPlay(const double _sample_rate, const int max_samples_per_block)
{
    sample_rate = _sample_rate;
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

void SynthAudioProcessor::addStateListener(SynthStateManager::Listener* new_listener)
{
    change_listener = new_listener;
}

void SynthAudioProcessor::removeStateListener(SynthStateManager::Listener* old_listener)
{
    change_listener = nullptr;
}

void SynthAudioProcessor::initState()
{
    state_manager.regenerateElementTrees();

    if(bit_synth != nullptr)
    {
        internal_state.generators.removeListener(bit_synth.get());
        internal_state.processors.removeListener(bit_synth.get());
        internal_state.sinks.removeListener(bit_synth.get());
    }

    bit_synth = std::make_unique<BitSynthesizer>(num_voices, state_manager);
    bit_synth->setCurrentPlaybackSampleRate(sample_rate);
    internal_state.generators.addListener(bit_synth.get());
    internal_state.processors.addListener(bit_synth.get());
    internal_state.sinks.addListener(bit_synth.get());
}

