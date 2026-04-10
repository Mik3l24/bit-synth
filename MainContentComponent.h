//
// Created by micha on 13.11.2023.
//
#pragma once

#include <juce_audio_utils/juce_audio_utils.h>

#include "SynthAudioSource.h"
#include "ui/StructureEditor.h"
#include "ui/TopBar.h"


class MainContentComponent : public juce::AudioAppComponent,
                             private juce::Timer,
                             private juce::MidiInputCallback,
                             public juce::Button::Listener
{
public:
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    void buttonClicked(juce::Button*) override;

    void resized() override;

public:
    MainContentComponent();
    ~MainContentComponent() override;

private:
    void timerCallback() override;
    void setMidiInput(int index);
    void handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message) override;

private:
    juce::MidiKeyboardState keyboardState;
    juce::AudioDeviceManager deviceManager;
    SynthAudioSource synthAudioSource;
    juce::MidiKeyboardComponent keyboardComponent;
    juce::MidiMessageCollector midiCollector;
    juce::ComboBox midiInputList;   
    std::unique_ptr<ui::StructureEditor> structure_editor;
    std::unique_ptr<ui::TopBar> top_bar;

    int lastInputIndex = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainContentComponent)
};
