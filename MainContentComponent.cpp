//
// Created by micha on 13.11.2023.
//
#include "MainContentComponent.h"

MainContentComponent::MainContentComponent()
    : keyboardComponent(keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard),
      synthAudioSource(keyboardState, midiCollector)

{
    addAndMakeVisible(keyboardComponent);

    addAndMakeVisible(midiInputList);
    midiInputList.setTextWhenNoChoicesAvailable("No MIDI Inputs Enabled");
    auto midiInputs = juce::MidiInput::getAvailableDevices();

    juce::StringArray midiInputNames;

    for(auto input : midiInputs)
        midiInputNames.add(input.name);

    midiInputList.addItemList(midiInputNames, 1);
    midiInputList.onChange = [this] { setMidiInput(midiInputList.getSelectedItemIndex()); };

    // find the first enabled device and use that by default
    for(auto input : midiInputs)
    {
        if(deviceManager.isMidiInputDeviceEnabled(input.identifier))
        {
            setMidiInput(midiInputs.indexOf(input));
            break;
        }
    }

    // if no enabled devices were found just use the first one in the list
    if(midiInputList.getSelectedId() == 0)
        setMidiInput(0);

    structure_editor = std::make_unique<ui::StructureEditor>(synthAudioSource.getBitSynth());
    addAndMakeVisible(*structure_editor);

    setSize(600, 830);
    setAudioChannels(0, 2);
    startTimer(400);
}

MainContentComponent::~MainContentComponent()
{
    shutdownAudio();
}

void MainContentComponent::resized()
{
    keyboardComponent.setBounds(10, getHeight() - 120, getWidth() - 20, 100);
    structure_editor->setBounds(0, 0, getWidth(), getHeight() - 120);
    midiInputList.setBounds(10, getHeight() - 25 , getWidth() - 20, 20);
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

void MainContentComponent::setMidiInput(int index)
{
    auto midiInputs = juce::MidiInput::getAvailableDevices();

    deviceManager.removeMidiInputDeviceCallback(midiInputs[lastInputIndex].identifier, this);
    deviceManager.removeMidiInputDeviceCallback(midiInputs[lastInputIndex].identifier, &midiCollector);
    auto newInput = midiInputs[index];

    if(!deviceManager.isMidiInputDeviceEnabled(newInput.identifier))
        deviceManager.setMidiInputDeviceEnabled(newInput.identifier, true);

    deviceManager.addMidiInputDeviceCallback(newInput.identifier, this);
    deviceManager.addMidiInputDeviceCallback(newInput.identifier, &midiCollector);
    midiInputList.setSelectedId(index + 1, juce::dontSendNotification);

    lastInputIndex = index;
}

void MainContentComponent::handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message)
{
    keyboardState.processNextMidiEvent(message);
}