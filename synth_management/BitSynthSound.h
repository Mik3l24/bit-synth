#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_data_structures/juce_data_structures.h>


// Class for holding the parameters of the synth
class BitSynthSound : public juce::SynthesiserSound
{
public: // Methods
    BitSynthSound() :
        parameters("BitSynth",
            {}, // Sound parameters here later
            {
                juce::ValueTree("Sources"),
                juce::ValueTree("Components"),
                juce::ValueTree("Sinks"),
            }
        )
        {
            // Copy references to subtrees
            sources = parameters.getChildWithName("Sources");
            components = parameters.getChildWithName("Components");
            sinks = parameters.getChildWithName("Sinks");
            jassert(sources.isValid());
            jassert(components.isValid());
            jassert(sinks.isValid());
        }
    // TODO static void fromXML(const juce::XmlElement& xml)

    bool appliesToNote    (int) override        { return true; }
    bool appliesToChannel (int) override        { return true; }

public: // Members
    juce::ValueTree parameters;
    // References to subtrees (note that juce::ValueTrees are copied by reference already!)
    juce::ValueTree sources;
    juce::ValueTree components;
    juce::ValueTree sinks;

    juce::UndoManager* undo_manager = nullptr;

};