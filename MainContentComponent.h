//
// Created by micha on 13.11.2023.
//
#pragma once

#include <juce_audio_utils/juce_audio_utils.h>

#include "SynthAudioSource.h"
#include "ui/StructureEditor.h"


class MainContentComponent : public juce::AudioAppComponent,
                             private juce::Timer,
                             public juce::DragAndDropContainer
{
public:
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void resized() override;

public:
    MainContentComponent();
    ~MainContentComponent() override;

private:
    void timerCallback() override;

private:
    juce::MidiKeyboardState keyboardState;
    SynthAudioSource synthAudioSource;
    juce::MidiKeyboardComponent keyboardComponent;
    std::unique_ptr<ui::StructureEditor> structure_editor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainContentComponent)
};
