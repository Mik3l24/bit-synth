//
// Created by micha on 13.11.2023.
//
#include "MainContentComponent.h"

MainContentComponent::MainContentComponent()
    : keyboardComponent(keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard),
      synthAudioSource(keyboardState)

{
    addAndMakeVisible(keyboardComponent);

    structure_editor = std::make_unique<ui::StructureEditor>(synthAudioSource.getBitSynth());
    addAndMakeVisible(*structure_editor);

    setSize(600, 800);
    setAudioChannels(0, 2);
    startTimer(400);
}

MainContentComponent::~MainContentComponent()
{
    shutdownAudio();
}

void MainContentComponent::resized()
{
    keyboardComponent.setBounds(10, getHeight() - 110, getWidth() - 20, 100);
    structure_editor->setBounds(0, 0, getWidth(), getHeight() - 120);
}

void MainContentComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    synthAudioSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainContentComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    synthAudioSource.getNextAudioBlock(bufferToFill);
}

void MainContentComponent::releaseResources()
{
    synthAudioSource.releaseResources();
}

void MainContentComponent::timerCallback()
{
    keyboardComponent.grabKeyboardFocus();
    stopTimer();
}