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

#include "OscillatorParameters.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
namespace ui {
//[/MiscUserDefs]

//==============================================================================
OscillatorParameters::OscillatorParameters(ConnectionID id, BitSynthesizer* synth)
    : SynthConnected(synth), id(id)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    pw_slider.reset (new juce::Slider ("pw_slider"));
    addAndMakeVisible (pw_slider.get());
    pw_slider->setRange (-0.5, 0.5, 0);
    pw_slider->setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    pw_slider->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    pw_slider->addListener (this);

    ratio_label.reset (new juce::Label ("ratio_label",
                                        TRANS("Ratio")));
    addAndMakeVisible (ratio_label.get());
    ratio_label->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    ratio_label->setJustificationType (juce::Justification::centred);
    ratio_label->setEditable (false, false, false);
    ratio_label->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    ratio_label->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    ratio_label->setBounds (32, 32, 70, 16);

    numerator_slider.reset (new juce::Slider ("numerator_slider"));
    addAndMakeVisible (numerator_slider.get());
    numerator_slider->setRange (1, 64, 1);
    numerator_slider->setSliderStyle (juce::Slider::LinearBarVertical);
    numerator_slider->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    numerator_slider->addListener (this);
    numerator_slider->setSkewFactor (0.5);

    numerator_slider->setBounds (24, 16, 32, 16);

    phase_slider.reset (new juce::Slider ("phase_knob"));
    addAndMakeVisible (phase_slider.get());
    phase_slider->setRange (0, 2, 0);
    phase_slider->setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    phase_slider->setTextBoxStyle (juce::Slider::NoTextBox, false, 80, 20);
    phase_slider->addListener (this);

    denominator_slider.reset (new juce::Slider ("denominator_slider"));
    addAndMakeVisible (denominator_slider.get());
    denominator_slider->setRange (1, 64, 1);
    denominator_slider->setSliderStyle (juce::Slider::LinearBarVertical);
    denominator_slider->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    denominator_slider->addListener (this);
    denominator_slider->setSkewFactor (0.5);

    denominator_slider->setBounds (80, 16, 32, 16);

    phase_label.reset (new juce::Label ("phase_label",
                                        TRANS("Phase")));
    addAndMakeVisible (phase_label.get());
    phase_label->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    phase_label->setJustificationType (juce::Justification::centred);
    phase_label->setEditable (false, false, false);
    phase_label->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    phase_label->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    pw_label.reset (new juce::Label ("pw_label",
                                     TRANS("Pulse Width")));
    addAndMakeVisible (pw_label.get());
    pw_label->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    pw_label->setJustificationType (juce::Justification::centred);
    pw_label->setEditable (false, false, false);
    pw_label->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    pw_label->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    transpose_slider.reset (new juce::Slider ("transpose_slider"));
    addAndMakeVisible (transpose_slider.get());
    transpose_slider->setRange (-36, 36, 1);
    transpose_slider->setSliderStyle (juce::Slider::LinearBar);
    transpose_slider->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 80, 20);
    transpose_slider->addListener (this);

    transpose_slider->setBounds (16, 56, 32, 16);

    transpose_label.reset (new juce::Label ("transpose_label",
                                            TRANS("Transpose")));
    addAndMakeVisible (transpose_label.get());
    transpose_label->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    transpose_label->setJustificationType (juce::Justification::centred);
    transpose_label->setEditable (false, false, false);
    transpose_label->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    transpose_label->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    transpose_label->setBounds (0, 80, 70, 16);

    fine_slider.reset (new juce::Slider ("fine_slider"));
    addAndMakeVisible (fine_slider.get());
    fine_slider->setTooltip (TRANS("cents"));
    fine_slider->setRange (-100, 100, 1);
    fine_slider->setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    fine_slider->setTextBoxStyle (juce::Slider::TextBoxLeft, false, 40, 20);
    fine_slider->addListener (this);

    fine_label.reset (new juce::Label ("fine_label",
                                       TRANS("Fine")));
    addAndMakeVisible (fine_label.get());
    fine_label->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    fine_label->setJustificationType (juce::Justification::centred);
    fine_label->setEditable (false, false, false);
    fine_label->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    fine_label->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    fine_label->setBounds (64, 80, 70, 16);

    source_connector.reset (new SourceConnector (id));
    addAndMakeVisible (source_connector.get());
    internalPath1.startNewSubPath (72.0f, 16.0f);
    internalPath1.lineTo (64.0f, 32.0f);
    internalPath1.closeSubPath();


    //[UserPreSize]
    //[/UserPreSize]

    setSize (136, 180);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

OscillatorParameters::~OscillatorParameters()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    pw_slider = nullptr;
    ratio_label = nullptr;
    numerator_slider = nullptr;
    phase_slider = nullptr;
    denominator_slider = nullptr;
    phase_label = nullptr;
    pw_label = nullptr;
    transpose_slider = nullptr;
    transpose_label = nullptr;
    fine_slider = nullptr;
    fine_label = nullptr;
    source_connector = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void OscillatorParameters::paint (juce::Graphics& g)
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

    {
        float x = 0, y = 0;
        juce::Colour strokeColour = juce::Colour (0xff818d92);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (strokeColour);
        g.strokePath (internalPath1, juce::PathStrokeType (2.700f, juce::PathStrokeType::mitered, juce::PathStrokeType::rounded), juce::AffineTransform::translation(x, y));
    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void OscillatorParameters::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    pw_slider->setBounds (proportionOfWidth (0.0368f), getHeight() - 90, 64, 72);
    phase_slider->setBounds (proportionOfWidth (0.5000f), getHeight() - 90, 64, 72);
    phase_label->setBounds (proportionOfWidth (0.5000f) + 64 / 2 - (48 / 2), (getHeight() - 90) + 62, 48, 16);
    pw_label->setBounds (proportionOfWidth (0.0368f) + 64 / 2 - (64 / 2), (getHeight() - 90) + 62, 64, 17);
    fine_slider->setBounds (proportionOfWidth (0.4706f), getHeight() - 132, 64, 32);
    source_connector->setBounds (getWidth() - 8, proportionOfHeight (0.5000f) - (10 / 2), 10, 10);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void OscillatorParameters::sliderValueChanged (juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == pw_slider.get())
    {
        //[UserSliderCode_pw_slider] -- add your slider handling code here..
        synth->setOscillatorPulseWidth(id, float(pw_slider->getValue()) + .5f); // Ofset from slider's [-0.5, 0.5] to [0, 1]
        //[/UserSliderCode_pw_slider]
    }
    else if (sliderThatWasMoved == numerator_slider.get())
    {
        //[UserSliderCode_numerator_slider] -- add your slider handling code here..
        synth->setOscillatorRatio(id, getRatio());
        //[/UserSliderCode_numerator_slider]
    }
    else if (sliderThatWasMoved == phase_slider.get())
    {
        //[UserSliderCode_phase_slider] -- add your slider handling code here..
        synth->setOscillatorStartingPhase(id,
                                          float(phase_slider->getValue()) * juce::MathConstants<float>::pi);
        //[/UserSliderCode_phase_slider]
    }
    else if (sliderThatWasMoved == denominator_slider.get())
    {
        //[UserSliderCode_denominator_slider] -- add your slider handling code here..
        synth->setOscillatorRatio(id, getRatio());
        //[/UserSliderCode_denominator_slider]
    }
    else if (sliderThatWasMoved == transpose_slider.get())
    {
        //[UserSliderCode_transpose_slider] -- add your slider handling code here..
        // TODO: implement in Oscillator
        //[/UserSliderCode_transpose_slider]
    }
    else if (sliderThatWasMoved == fine_slider.get())
    {
        //[UserSliderCode_fine_slider] -- add your slider handling code here..
        // TODO: implement in Oscillator
        //[/UserSliderCode_fine_slider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void OscillatorParameters::mouseDown(const juce::MouseEvent& e)
{
    dragger.startDraggingComponent(this, e);
}

void OscillatorParameters::mouseDrag(const juce::MouseEvent& e)
{
    dragger.dragComponent(this, e, nullptr);
    if(e.mouseWasDraggedSinceMouseDown())
        // I guess this is a bit of a hack to repaint the StructureEditor
        if(auto* parent = dynamic_cast<juce::Component*>(juce::DragAndDropContainer::findParentDragContainerFor(this)))
            parent->repaint();
}

inline double OscillatorParameters::getRatio() const
{
    return numerator_slider->getValue() / denominator_slider->getValue();
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="OscillatorParameters" componentName=""
                 parentClasses="public juce::Component" constructorParams="ConnectionID id"
                 variableInitialisers="id(id)&#10;" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="1" initialWidth="136"
                 initialHeight="180">
  <BACKGROUND backgroundColour="ff505050">
    <RECT pos="0 0 100% 100%" fill="solid: ffc4c4c4" hasStroke="1" stroke="4.4, mitered, butt"
          strokeColour="solid: ff787878"/>
    <PATH pos="0 0 100 100" fill="solid: 2a38a5" hasStroke="1" stroke="2.7, mitered, rounded"
          strokeColour="solid: ff818d92" nonZeroWinding="1">s 72 16 l 64 32 x</PATH>
  </BACKGROUND>
  <SLIDER name="new slider" id="639f5f06058944aa" memberName="pw_slider"
          virtualName="" explicitFocusOrder="0" pos="3.676% 90R 64 72"
          min="-0.5" max="0.5" int="0.0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="new label" id="cd9c8c6719b6d018" memberName="ratio_label"
         virtualName="" explicitFocusOrder="0" pos="32 32 70 16" edTextCol="ff000000"
         edBkgCol="0" labelText="Ratio" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="new slider" id="e1f0206cf686f9d3" memberName="numerator_slider"
          virtualName="" explicitFocusOrder="0" pos="24 16 32 16" min="1.0"
          max="64.0" int="1.0" style="LinearBarVertical" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="0.5"
          needsCallback="1"/>
  <SLIDER name="phase_knob" id="7c123bc2b98d85c2" memberName="phase_slider"
          virtualName="" explicitFocusOrder="0" pos="50% 90R 64 72" min="0.0"
          max="2.0" int="0.0" style="RotaryHorizontalVerticalDrag" textBoxPos="NoTextBox"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="new slider" id="6682d7401c24afc" memberName="denominator_slider"
          virtualName="" explicitFocusOrder="0" pos="80 16 32 16" min="1.0"
          max="64.0" int="1.0" style="LinearBarVertical" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="0.5"
          needsCallback="1"/>
  <LABEL name="new label" id="ab17c20d2f7abdb3" memberName="phase_label"
         virtualName="" explicitFocusOrder="0" pos="0Cc 62 48 16" posRelativeX="7c123bc2b98d85c2"
         posRelativeY="7c123bc2b98d85c2" edTextCol="ff000000" edBkgCol="0"
         labelText="Phase" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="36"/>
  <LABEL name="new label" id="d4655e3385df5412" memberName="pw_label"
         virtualName="" explicitFocusOrder="0" pos="0Cc 62 64 17" posRelativeX="639f5f06058944aa"
         posRelativeY="639f5f06058944aa" edTextCol="ff000000" edBkgCol="0"
         labelText="Pulse Width" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="new slider" id="1638c8fd5445b4b4" memberName="transpose_slider"
          virtualName="" explicitFocusOrder="0" pos="16 56 32 16" min="-36.0"
          max="36.0" int="1.0" style="LinearBar" textBoxPos="TextBoxLeft"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <LABEL name="new label" id="ac3d8955bab0e6fe" memberName="transpose_label"
         virtualName="" explicitFocusOrder="0" pos="0 80 70 16" edTextCol="ff000000"
         edBkgCol="0" labelText="Transpose" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="36"/>
  <SLIDER name="phase_knob" id="3b789c491e12f4a2" memberName="fine_slider"
          virtualName="" explicitFocusOrder="0" pos="47.059% 132R 64 32"
          tooltip="cents" min="-100.0" max="100.0" int="1.0" style="RotaryHorizontalVerticalDrag"
          textBoxPos="TextBoxLeft" textBoxEditable="1" textBoxWidth="40"
          textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <LABEL name="new label" id="9758f65a2ebe76a9" memberName="fine_label"
         virtualName="" explicitFocusOrder="0" pos="64 80 70 16" edTextCol="ff000000"
         edBkgCol="0" labelText="Fine" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="36"/>
  <JUCERCOMP name="" id="c4fa963da602e7ee" memberName="source_connector" virtualName=""
             explicitFocusOrder="0" pos="8R 50%c 10 10" sourceFile="SourceConnector.cpp"
             constructorParams="id"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
}
//[/EndFile]

