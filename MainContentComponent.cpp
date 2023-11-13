//
// Created by micha on 13.11.2023.
//
#include "MainContentComponent.h"

MainContentComponent::MainContentComponent()
    : keyboardComponent(keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard),
      synthAudioSource(keyboardState)

{
    addAndMakeVisible(keyboardComponent);
    setSize(600, 160);
    setAudioChannels(0, 2);
    startTimer(400);
}

MainContentComponent::~MainContentComponent()
{
    shutdownAudio();
}

void MainContentComponent::resized()
{
    keyboardComponent.setBounds(10, 10, getWidth() - 20, getHeight() - 20);
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