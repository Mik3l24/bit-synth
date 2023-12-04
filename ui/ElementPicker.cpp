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
//[/Headers]

#include "ElementPicker.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
namespace ui
{
//[/MiscUserDefs]

//==============================================================================
    ElementPicker::ElementPicker()
    {
        //[Constructor_pre] You can add your own custom stuff here..
        //[/Constructor_pre]

        osc_button = std::make_unique<ElementAdderButton>(ElementType::OSCILLATOR);
        addAndMakeVisible(osc_button.get());
        osc_button->setButtonText(TRANS("Osc"));
        osc_button->addListener(this);

        osc_button->setBounds(0, 0, 48, 24);

        not_button = std::make_unique<ElementAdderButton>(GateType::NOT);
        addAndMakeVisible(not_button.get());
        not_button->setButtonText(TRANS("NOT"));
        not_button->addListener(this);

        not_button->setBounds(48, 0, 48, 24);

        and_button = std::make_unique<ElementAdderButton>(GateType::AND);
        addAndMakeVisible(and_button.get());
        and_button->setButtonText(TRANS("AND"));
        and_button->addListener(this);

        and_button->setBounds(96, 0, 48, 24);

        or_button = std::make_unique<ElementAdderButton>(GateType::OR);
        addAndMakeVisible(or_button.get());
        or_button->setButtonText(TRANS("OR"));
        or_button->addListener(this);

        or_button->setBounds(144, 0, 48, 24);

        xor_button = std::make_unique<ElementAdderButton>(GateType::XOR);
        addAndMakeVisible(xor_button.get());
        xor_button->setButtonText(TRANS("XOR"));
        xor_button->addListener(this);

        xor_button->setBounds(192, 0, 48, 24);

        mix_button = std::make_unique<ElementAdderButton>(ElementType::MIX_CHANNEL);
        addAndMakeVisible(mix_button.get());
        mix_button->setButtonText(TRANS("Mix"));
        mix_button->addListener(this);

        mix_button->setBounds(240, 0, 48, 24);


        //[UserPreSize]
        //[/UserPreSize]

        setSize(288, 24);


        //[Constructor] You can add your own custom stuff here..
        //[/Constructor]
    }

    ElementPicker::~ElementPicker()
    {
        //[Destructor_pre]. You can add your own custom destruction code here..
        //[/Destructor_pre]

        osc_button = nullptr;
        not_button = nullptr;
        and_button = nullptr;
        or_button = nullptr;
        xor_button = nullptr;
        mix_button = nullptr;


        //[Destructor]. You can add your own custom destruction code here..
        //[/Destructor]
    }

//==============================================================================
    void ElementPicker::paint(juce::Graphics& g)
    {
        //[UserPrePaint] Add your own custom painting code here..
        //[/UserPrePaint]


        //[UserPaint] Add your own custom painting code here..
        //[/UserPaint]
    }

    void ElementPicker::resized()
    {
        //[UserPreResize] Add your own custom resize code here..
        //[/UserPreResize]

        //[UserResized] Add your own custom resize handling here..
        //[/UserResized]
    }

    void ElementPicker::buttonClicked(juce::Button* buttonThatWasClicked)
    {
        //[UserbuttonClicked_Pre]
        //[/UserbuttonClicked_Pre]

        if (buttonThatWasClicked == osc_button.get()) {
            //[UserButtonCode_osc_button] -- add your button handler code here..
            //[/UserButtonCode_osc_button]
        } else if (buttonThatWasClicked == not_button.get()) {
            //[UserButtonCode_not_button] -- add your button handler code here..
            //[/UserButtonCode_not_button]
        } else if (buttonThatWasClicked == and_button.get()) {
            //[UserButtonCode_and_button] -- add your button handler code here..
            //[/UserButtonCode_and_button]
        } else if (buttonThatWasClicked == or_button.get()) {
            //[UserButtonCode_or_button] -- add your button handler code here..
            //[/UserButtonCode_or_button]
        } else if (buttonThatWasClicked == xor_button.get()) {
            //[UserButtonCode_xor_button] -- add your button handler code here..
            //[/UserButtonCode_xor_button]
        } else if (buttonThatWasClicked == mix_button.get()) {
            //[UserButtonCode_mix_button] -- add your button handler code here..
            //[/UserButtonCode_mix_button]
        }

        //[UserbuttonClicked_Post]
        //[/UserbuttonClicked_Post]
    }



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
    /*  -- Projucer information section --

        This is where the Projucer stores the metadata that describe this GUI layout, so
        make changes in here at your peril!

    BEGIN_JUCER_METADATA

    <JUCER_COMPONENT documentType="Component" className="ElementPicker" componentName=""
                     parentClasses="public juce::Component" constructorParams="" variableInitialisers=""
                     snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                     fixedSize="1" initialWidth="288" initialHeight="24">
      <BACKGROUND backgroundColour="ff505050"/>
      <TEXTBUTTON name="new button" id="3e01de4dbc443588" memberName="osc_button"
                  virtualName="" explicitFocusOrder="0" pos="0 0 48 24" buttonText="Osc"
                  connectedEdges="0" needsCallback="1" radioGroupId="0"/>
      <TEXTBUTTON name="new button" id="355beec98d15b647" memberName="not_button"
                  virtualName="" explicitFocusOrder="0" pos="48 0 48 24" buttonText="NOT"
                  connectedEdges="0" needsCallback="1" radioGroupId="0"/>
      <TEXTBUTTON name="new button" id="520f8457675c8e3e" memberName="and_button"
                  virtualName="" explicitFocusOrder="0" pos="96 0 48 24" buttonText="AND"
                  connectedEdges="0" needsCallback="1" radioGroupId="0"/>
      <TEXTBUTTON name="new button" id="6a2815ebf14f59fe" memberName="or_button"
                  virtualName="" explicitFocusOrder="0" pos="144 0 48 24" buttonText="OR"
                  connectedEdges="0" needsCallback="1" radioGroupId="0"/>
      <TEXTBUTTON name="new button" id="b34fcf45e4b67aaa" memberName="xor_button"
                  virtualName="" explicitFocusOrder="0" pos="192 0 48 24" buttonText="XOR"
                  connectedEdges="0" needsCallback="1" radioGroupId="0"/>
      <TEXTBUTTON name="new button" id="240161126f27fc84" memberName="mix_button"
                  virtualName="" explicitFocusOrder="0" pos="240 0 48 24" buttonText="Mix"
                  connectedEdges="0" needsCallback="1" radioGroupId="0"/>
    </JUCER_COMPONENT>

    END_JUCER_METADATA
    */
#endif


//[EndFile] You can add extra defines here...
}
//[/EndFile]

