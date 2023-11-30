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

//[Headers] You can add your own extra header files here...
#include "DragSourceType.h"
#include "Theme.h"
//[/Headers]

#include "StructureEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
namespace ui {
//[/MiscUserDefs]

//==============================================================================
StructureEditor::StructureEditor(BitSynthesizer* synth)
    : SynthConnected(synth)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    picker.reset (new ElementPicker());
    addAndMakeVisible (picker.get());

    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..

    //[/Constructor]
}

StructureEditor::~StructureEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    picker = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void StructureEditor::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Theme::getStructureBackground());

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void StructureEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    picker->setBounds ((getWidth() / 2) - (proportionOfWidth (0.6000f) / 2), 0, proportionOfWidth (0.6000f), 24);
    //[UserResized] Add your own custom resize handling here..
    auto parentDragContainer = juce::DragAndDropContainer::findParentDragContainerFor(this);
    if(parentDragContainer != nullptr)
        parentDragContainer->isDragAndDropActive();
    //[/UserResized]
}


//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
bool StructureEditor::isInterestedInDragSource(const juce::DragAndDropTarget::SourceDetails& dragSourceDetails)
{
    return dragSourceDetails.description.equals(DragSourceType::ELEMENT_ADDER_BUTTON);
}

void StructureEditor::itemDropped(const juce::DragAndDropTarget::SourceDetails& dragSourceDetails)
{
    auto* button = dynamic_cast<ElementAdderButton*>(dragSourceDetails.sourceComponent.get());
    if(button == nullptr)
    {
        std::cout << "StructureEditor::itemDropped: source is not ElementAdderButton" << std::endl;
        return;
    }

    juce::Component* component = nullptr;
    switch(button->getElementType())
    {
        ConnectionID id;
        case ElementType::OSCILLATOR:
        {
            id = synth->addOscillator();
            osc_components.emplace_back(new OscillatorParameters(id, synth));
            component = osc_components.back().get();
            break;
        }
        case ElementType::GATE:
        {
            GateType gate_type = button->getGateType();
            id = synth->addGate(gate_type);
            gate_components.emplace_back(new Gate(id, gate_type, synth));
            component = gate_components.back().get();
            break;
        }
        case ElementType::MIX_CHANNEL:
        {
            id = synth->addMixChannel();
            mix_components.emplace_back(new MixChannelParameters(id, synth));
            component = mix_components.back().get();
            break;
        }

    }
    if(component == nullptr)
    {
        std::cout << "StructureEditor::itemDropped: component is nullptr" << std::endl;
        return;
    }

    // Add the component and place it at the mouse position
    addAndMakeVisible(component);
    component->setCentrePosition(dragSourceDetails.localPosition);

}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="StructureEditor" componentName=""
                 parentClasses="public juce::Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff505050"/>
  <JUCERCOMP name="" id="bd33bf19f618d685" memberName="picker" virtualName=""
             explicitFocusOrder="0" pos="0Cc 0 60% 24" sourceFile="ElementPicker.cpp"
             constructorParams=""/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
}
//[/EndFile]

