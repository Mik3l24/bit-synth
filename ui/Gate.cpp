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

#include "Gate.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
namespace ui {
//[/MiscUserDefs]

//==============================================================================
Gate::Gate(ConnectionID id, GateType type, BitSynthesizer* synth)
    : SynthConnected(synth), type(type)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    source.reset (new SourceConnector (id));
    addAndMakeVisible (source.get());
    source->setName ("new component");

    source->setBounds (80, 46, 10, 10);

    target0.reset (new TargetConnector(id));
    addAndMakeVisible (target0.get());
    target0->setName ("new component");

    target0->setBounds (8, 36, 10, 10);

    target1.reset (new TargetConnector(id));
    addAndMakeVisible (target1.get());
    target1->setName ("new component");

    target1->setBounds (8, 55, 10, 10);

    internalPath1.startNewSubPath (30.0f, 30.0f);
    internalPath1.lineTo (54.0f, 30.0f);
    internalPath1.quadraticTo (72.0f, 30.0f, 72.0f, 50.0f);
    internalPath1.quadraticTo (72.0f, 70.0f, 54.0f, 70.0f);
    internalPath1.lineTo (30.0f, 70.0f);
    internalPath1.closeSubPath();

    internalPath2.setUsingNonZeroWinding (false);
    internalPath2.startNewSubPath (13.0f, 59.0f);
    internalPath2.lineTo (28.0f, 59.0f);
    internalPath2.closeSubPath();

    internalPath3.startNewSubPath (72.0f, 51.0f);
    internalPath3.lineTo (86.0f, 51.0f);
    internalPath3.closeSubPath();

    internalPath4.setUsingNonZeroWinding (false);
    internalPath4.startNewSubPath (13.0f, 41.0f);
    internalPath4.lineTo (29.0f, 41.0f);
    internalPath4.closeSubPath();


    //[UserPreSize]
    //[/UserPreSize]

    setSize (100, 100);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

Gate::~Gate()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    source = nullptr;
    target0 = nullptr;
    target1 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void Gate::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    {
        float x = 0, y = 0;
        juce::Colour strokeColour = juce::Colour (0xff505050);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (strokeColour);
        g.strokePath (internalPath1, juce::PathStrokeType (5.000f), juce::AffineTransform::translation(x, y));
    }

    {
        float x = 0, y = 0;
        juce::Colour strokeColour = juce::Colour (0xff505050);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (strokeColour);
        g.strokePath (internalPath2, juce::PathStrokeType (5.000f), juce::AffineTransform::translation(x, y));
    }

    {
        float x = 0, y = 0;
        juce::Colour strokeColour = juce::Colour (0xff505050);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (strokeColour);
        g.strokePath (internalPath3, juce::PathStrokeType (5.000f), juce::AffineTransform::translation(x, y));
    }

    {
        float x = 0, y = 0;
        juce::Colour strokeColour = juce::Colour (0xff505050);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (strokeColour);
        g.strokePath (internalPath4, juce::PathStrokeType (5.000f), juce::AffineTransform::translation(x, y));
    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void Gate::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="Gate" componentName="" parentClasses="public juce::Component"
                 constructorParams="int type, int id" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="100" initialHeight="100">
  <BACKGROUND backgroundColour="505050">
    <PATH pos="0 0 100 100" fill="solid: 35a52a" hasStroke="1" stroke="5, mitered, butt"
          strokeColour="solid: ff505050" nonZeroWinding="1">s 30 30 l 54 30 q 72 30 72 50 q 72 70 54 70 l 30 70 x</PATH>
    <PATH pos="0 0 100 100" fill="solid: 35a52a" hasStroke="1" stroke="5, mitered, butt"
          strokeColour="solid: ff505050" nonZeroWinding="0">s 13 59 l 28 59 x</PATH>
    <PATH pos="0 0 100 100" fill="solid: 35a52a" hasStroke="1" stroke="5, mitered, butt"
          strokeColour="solid: ff505050" nonZeroWinding="1">s 72 51 l 86 51 x</PATH>
    <PATH pos="0 0 100 100" fill="solid: 35a52a" hasStroke="1" stroke="5, mitered, butt"
          strokeColour="solid: ff505050" nonZeroWinding="0">s 13 41 l 29 41 x</PATH>
  </BACKGROUND>
  <GENERICCOMPONENT name="new component" id="d654ae1065101ce0" memberName="source"
                    virtualName="" explicitFocusOrder="0" pos="80 46 10 10" class="SourceConnector"
                    params="id"/>
  <GENERICCOMPONENT name="new component" id="2dac8186f31e2ec6" memberName="target0"
                    virtualName="" explicitFocusOrder="0" pos="8 36 10 10" class="TargetConnector"
                    params=""/>
  <GENERICCOMPONENT name="new component" id="aa9cd0ecff1a05d6" memberName="target1"
                    virtualName="" explicitFocusOrder="0" pos="8 55 10 10" class="TargetConnector"
                    params=""/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
}
//[/EndFile]

