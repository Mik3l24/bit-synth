/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 7.0.5

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include <juce_gui_basics/juce_gui_basics.h>

#include "ElementPicker.h"
#include "Gate.h"
#include "OscillatorParameters.h"
#include "MixChannelParameters.h"
#include "synth_management/SynthStateManager.h"

namespace ui {
//[/Headers]




//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class StructureEditor:
    public juce::Component,
    public juce::DragAndDropTarget,
    public juce::DragAndDropContainer
{
public:
    //==============================================================================
    explicit StructureEditor(const SynthStateManager& state_manager);
    ~StructureEditor() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    // Overrides
    bool isInterestedInDragSource(const juce::DragAndDropTarget::SourceDetails& dragSourceDetails) override;
    void itemDropped(const juce::DragAndDropTarget::SourceDetails& dragSourceDetails) override;
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;

    void rebuildFromTree();

private: // Methods
    void addNewElement(const juce::Point<int> position, const ElementType element_type, const GateType gate_type = GateType::NONE)
    {
        const ElementID id = state_manager.addElementRep(element_type, gate_type);
        addElementComponent(id, position, element_type, gate_type);
    }

    void addElementComponent(ElementID id, juce::Point<int> position, ElementType element_type, GateType gate_type = GateType::NONE);

    [[nodiscard]] OscillatorParameters* findGeneratorByID(ElementID id) const;
    [[nodiscard]]                 Gate* findComponentByID(ElementID id) const;
    [[nodiscard]] MixChannelParameters* findSinkByID(ElementID id) const;

    [[nodiscard]] SourceConnector* findSourceByID(ConnectionID id) const;


private: // Members
    //[UserVariables]   -- You can add your own custom variables in this section.
    std::vector<std::unique_ptr<OscillatorParameters>> osc_components;
    std::vector<std::unique_ptr<Gate>> gate_components;
    std::vector<std::unique_ptr<MixChannelParameters>> mix_components;

    SynthStateManager state_manager;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<ElementPicker> picker;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StructureEditor)
};

//[EndFile] You can add extra defines here...
}
//[/EndFile]

