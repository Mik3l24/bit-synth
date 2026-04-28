//
// Created by micha on 12.04.2026.
//

#include "SynthEditor.h"
#include "synth_management/SynthManagementNames.h"
#include "synth_management/RepresentationStructs.h"

using namespace ui;

SynthEditor::SynthEditor(juce::AudioProcessor* _parent, SynthStateManager& state_manager, SynthStateManager::StateChangeSender* _sender)
        : AudioProcessorEditor(_parent),
          sender(_sender),
          state_manager(state_manager)
{
    jassert(_parent != nullptr);
    top_bar = std::make_unique<ui::TopBar>(this);
    addAndMakeVisible(*top_bar);

    structure_editor = std::make_unique<StructureEditor>(state_manager);
    addAndMakeVisible(*structure_editor);

    setResizeLimits(600, 800,1920, 1080);
    setResizable(true, true);

    setSize(600, 800);

    if(sender != nullptr)
        sender->addStateListener(this);
}

SynthEditor::~SynthEditor()
{
    if(sender != nullptr)
        sender->removeStateListener(this);
}

void SynthEditor::resized()
{
    AudioProcessorEditor::resized();
    top_bar->setBounds(0, 0, getWidth(), 20);
    structure_editor->setBounds(0, 20, getWidth(), getHeight() - 20);
}

void SynthEditor::buttonClicked(juce::Button* button)
{
    static std::unique_ptr<juce::FileChooser> chooser;

    if(button == top_bar->save_button.get())
    {
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
            saveStateToFile(selected_file);
        });
    }
    else if(button == top_bar->load_button.get())
    {
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
            loadStateFromFile(selected_file);
        });
    }
}

void SynthEditor::saveStateToFile(const juce::File& file) const
{
    const auto state = state_manager.parameters.copyState();
    if(file == juce::File())
        return;
    const auto xml(state.createXml());
    jassert(xml != nullptr);
    const bool _ = file.replaceWithText("");
    jassert(_);
    if(!xml->writeTo(file, {}))
        std::cerr << "Error: Failed to write synth preset to file\n";
}

void SynthEditor::loadStateFromFile(const juce::File& file)
{
    if(file == juce::File() || !file.existsAsFile())
        return;
    const auto xml = juce::XmlDocument::parse(file);
    if(xml.get() == nullptr || !xml->hasTagName(state_manager.parameters.state.getType()))
        return;
    state_manager.parameters.replaceState(juce::ValueTree::fromXml(*xml));
}

void SynthEditor::stateReplaced()
{
    rebuildUI();
}

void SynthEditor::rebuildUI() const
{
    structure_editor->rebuildUI();
}
