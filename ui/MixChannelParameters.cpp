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

#include "MixChannelParameters.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
namespace ui {
//[/MiscUserDefs]

//==============================================================================
MixChannelParameters::MixChannelParameters (ConnectionID id, BitSynthesizer* synth)
    : SynthConnected(synth), id(id)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    level_slider.reset (new juce::Slider ("phase_knob"));
    addAndMakeVisible (level_slider.get());
    level_slider->setRange (0, 1, 0);
    level_slider->setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    level_slider->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    level_slider->addListener (this);

    level_label.reset (new juce::Label ("new label",
                                        TRANS("Level")));
    addAndMakeVisible (level_label.get());
    level_label->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    level_label->setJustificationType (juce::Justification::centred);
    level_label->setEditable (false, false, false);
    level_label->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    level_label->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    target.reset (new TargetConnector (id));
    addAndMakeVisible (target.get());
    target->addListener(this);
    target->setBounds (0, 40, 10, 10);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (96, 96);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

MixChannelParameters::~MixChannelParameters()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    level_slider = nullptr;
    level_label = nullptr;
    target = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void MixChannelParameters::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff505050));

    {
        int x = 0, y = 0, width = proportionOfWidth (1.0000f), height = proportionOfHeight (1.0000f);
        juce::Colour fillColour = juce::Colour (0xffc4c4c4);
        juce::Colour strokeColour = juce::Colour (0xff787878);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (fillColour);
        g.fillRect (x, y, width, height);
        g.setColour (strokeColour);
        g.drawRect (x, y, width, height, 4);

    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void MixChannelParameters::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    level_slider->setBounds (proportionOfWidth (0.1667f), getHeight() - 96, 64, 72);
    level_label->setBounds (proportionOfWidth (0.1667f) + 64 / 2 - (48 / 2), (getHeight() - 96) + 64, 48, 16);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void MixChannelParameters::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == level_slider.get())
    {
        //[UserSliderCode_level_slider] -- add your slider handling code here..
        synth->setMixChannelLevel(id, float(level_slider->getValue()));
        //[/UserSliderCode_level_slider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void MixChannelParameters::mouseDown(const juce::MouseEvent& e)
{
    dragger.startDraggingComponent(this, e);
}

void MixChannelParameters::mouseDrag(const juce::MouseEvent& e)
{
    dragger.dragComponent(this, e, nullptr);
}


void MixChannelParameters::connectionMade(TargetConnector* connector, ConnectionID source_id)
{
    if(connector == target.get())
    {
        synth->setMixChannelInput(id, source_id);
    }
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MixChannelParameters" componentName=""
                 parentClasses="public juce::Component" constructorParams="ConnectionID id"
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="1" initialWidth="96" initialHeight="96">
  <BACKGROUND backgroundColour="ff505050">
    <RECT pos="0 0 100% 100%" fill="solid: ffc4c4c4" hasStroke="1" stroke="4.4, mitered, butt"
          strokeColour="solid: ff787878"/>
  </BACKGROUND>
  <SLIDER name="phase_knob" id="7c123bc2b98d85c2" memberName="level_slider"
          virtualName="" explicitFocusOrder="0" pos="16.667% 96R 64 72"
          min="0.0" max="1.0" int="0.0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="new label" id="ab17c20d2f7abdb3" memberName="level_label"
         virtualName="" explicitFocusOrder="0" pos="0Cc 64 48 16" posRelativeX="7c123bc2b98d85c2"
         posRelativeY="7c123bc2b98d85c2" edTextCol="ff000000" edBkgCol="0"
         labelText="Level" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="36"/>
  <JUCERCOMP name="" id="45d6bb79d761a689" memberName="target" virtualName=""
             explicitFocusOrder="0" pos="0 40 10 10" sourceFile="TargetConnector.cpp"
             constructorParams="id"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
}
//[/EndFile]

