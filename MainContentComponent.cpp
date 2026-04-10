//
// Created by micha on 13.11.2023.
//
#include "MainContentComponent.h"

#include "ui/Theme.h"

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

    top_bar = std::make_unique<ui::TopBar>(this);
    addAndMakeVisible(*top_bar);

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
    top_bar->setBounds(0, 0, getWidth(), 20);
    keyboardComponent.setBounds(10, getHeight() - 120, getWidth() - 20, 100);
    structure_editor->setBounds(0, 20, getWidth(), getHeight() - 140);
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

void MainContentComponent::buttonClicked(juce::Button* button)
{
    static std::unique_ptr<juce::FileChooser> chooser;

    if(button == top_bar->save_button.get())
    {
        std::cout << "Saving..." << std::endl;

        chooser = std::make_unique<juce::FileChooser>(
            TRANS("Select a location to save your synth preset"),
            juce::File(), "*.xml"
            );
        constexpr auto chooser_flags = juce::FileBrowserComponent::saveMode | juce::FileBrowserComponent::canSelectFiles;

        chooser->launchAsync(chooser_flags, [this](const juce::FileChooser& chooser)
        {
            juce::File selected_file = chooser.getResult();
            std::cout << selected_file.getFullPathName() << std::endl;
            if(selected_file == juce::File()) return;

            auto* synth = synthAudioSource.getBitSynth();
            auto xml = synth->toXml();
            jassert(xml != nullptr);
            if(!xml->writeTo(selected_file, {}))
                std::cerr << "Error: Failed to write synth preset to file\n";
        });
    }
    else if(button == top_bar->load_button.get())
    {
        std::cout << "Loading..." << std::endl;

        chooser = std::make_unique<juce::FileChooser>(
            TRANS("Select a synth preset to load"),
            juce::File(), "*.xml"
            );
        constexpr auto chooser_flags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

        chooser->launchAsync(chooser_flags, [this](const juce::FileChooser& chooser)
        {
            juce::File selected_file = chooser.getResult();
            std::cout << selected_file.getFullPathName() << std::endl;
            if(selected_file == juce::File() || !selected_file.existsAsFile()) return;

            auto xml = juce::XmlDocument::parse(selected_file);
            jassert(xml != nullptr);

            auto* synth = synthAudioSource.getBitSynth();
            synth->fromXml(*xml);
        });
    }
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