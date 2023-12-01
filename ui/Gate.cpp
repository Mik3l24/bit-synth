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
#include "Theme.h"
//[/Headers]

#include "Gate.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
namespace ui {
//[/MiscUserDefs]

//==============================================================================
Gate::Gate(ConnectionID id, GateType type, BitSynthesizer* synth)
    : SynthConnected(synth), type(type), id(id)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    source.reset (new SourceConnector (id));
    addAndMakeVisible (source.get());
    source->setName ("source");

    source->setCentrePosition(86, 50);

    target0.reset (new TargetConnector(id));
    addAndMakeVisible (target0.get());
    target0->setName ("target0");
    target0->addListener(this);

    if(type != GateType::NOT)
    {
        target0->setCentrePosition(13, 41);

        target1.reset(new TargetConnector(id));
        addAndMakeVisible(target1.get());
        target1->setName("target1");
        target1->addListener(this);

        target1->setCentrePosition(13, 59);
    }
    else // if(type == GateType::NOT)
    {
        target0->setCentrePosition(13, 50);
        target1 = nullptr;
    }

    switch(type) // Body
    {
    case GateType::AND:
        bodyPath.startNewSubPath(30.0f, 30.0f);
        bodyPath.lineTo(54.0f, 30.0f);
        bodyPath.quadraticTo(72.0f, 30.0f, 72.0f, 50.0f);
        bodyPath.quadraticTo(72.0f, 70.0f, 54.0f, 70.0f);
        bodyPath.lineTo(30.0f, 70.0f);
        bodyPath.closeSubPath();
        break;
    case GateType::XOR:
        // The additional curve
        bodyPath.startNewSubPath (19.0f, 69.0f);
        bodyPath.quadraticTo(28.0f, 63.0f, 28.0f, 49.0f);
        bodyPath.quadraticTo(28.0f, 37.0f, 20.0f, 30.0f);
    case GateType::OR:
        // The common _or body
        bodyPath.startNewSubPath (45.0f, 29.0f);
        bodyPath.quadraticTo(64.0f, 29.0f, 70.0f, 50.0f);
        bodyPath.quadraticTo(63.0f, 70.0f, 46.0f, 70.0f);
        bodyPath.lineTo(28.0f, 70.0f);
        bodyPath.quadraticTo(36.0f, 63.0f, 36.0f, 49.0f);
        bodyPath.quadraticTo(36.0f, 37.0f, 29.0f, 29.0f);
        bodyPath.closeSubPath();
        break;
    case GateType::NOT:
        bodyPath.startNewSubPath (28.0f, 33.0f);
        bodyPath.lineTo(63.0f, 50.0f);
        bodyPath.lineTo(28.0f, 67.0f);
        bodyPath.closeSubPath();
        break;
    default:
        // Crossed out
        bodyPath.startNewSubPath (30.0f, 30.0f);
        bodyPath.lineTo(70.0f, 70.0f);
        bodyPath.closeSubPath();
        bodyPath.startNewSubPath (30.0f, 70.0f);
        bodyPath.lineTo(70.0f, 30.0f);
        bodyPath.closeSubPath();
        break;
    }

    switch(type) // Target tips
    {
    case GateType::AND:
    case GateType::XOR:
        // target1 tip
        connectorsPath.setUsingNonZeroWinding(false);
        connectorsPath.startNewSubPath(13.0f, 59.0f);
        connectorsPath.lineTo(27.0f, 59.0f);
        connectorsPath.closeSubPath();

        // target0 tip
        connectorsPath.setUsingNonZeroWinding(false);
        connectorsPath.startNewSubPath(13.0f, 41.0f);
        connectorsPath.lineTo(27.0f, 41.0f);
        connectorsPath.closeSubPath();
        break;
    case GateType::OR: // OR tips are slightly longer
        connectorsPath.setUsingNonZeroWinding (false);
        connectorsPath.startNewSubPath(13.0f, 59.0f);
        connectorsPath.lineTo(34.0f, 59.0f);
        connectorsPath.closeSubPath();

        connectorsPath.setUsingNonZeroWinding (false);
        connectorsPath.startNewSubPath(13.0f, 41.0f);
        connectorsPath.lineTo(34.0f, 41.0f);
        connectorsPath.closeSubPath();
        break;
    case GateType::NOT:
        connectorsPath.setUsingNonZeroWinding (false);
        connectorsPath.startNewSubPath(13.0f, 50.0f);
        connectorsPath.lineTo(26.0f, 50.0f);
        connectorsPath.closeSubPath();
        break;
    default:
        break;
    }



    // Source tip
    connectorsPath.startNewSubPath (72.0f, 50.0f);
    connectorsPath.lineTo (86.0f, 50.0f);
    connectorsPath.closeSubPath();



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
    juce::Colour strokeColour = Theme::getStructureLogicForeground();
    juce::Colour fillColour = Theme::getStructureBackground();
    float strokeThickness = Theme::getStructureLogicStrokeThickness();
    //[/UserPrePaint]



    //[UserPaint] Add your own custom painting code here..
    g.setColour(strokeColour);

    g.strokePath(bodyPath,
                 juce::PathStrokeType(strokeThickness, juce::PathStrokeType::JointStyle::curved, juce::PathStrokeType::EndCapStyle::rounded),
                 juce::AffineTransform::translation(0, 0));
    g.strokePath(connectorsPath,
                 juce::PathStrokeType(strokeThickness, juce::PathStrokeType::JointStyle::curved, juce::PathStrokeType::EndCapStyle::rounded),
                 juce::AffineTransform::translation(0, 0));

    if(type == GateType::NOT)
    {
        constexpr float x = 61.0f, y = 45.0f, width = 10.0f, height = 10.0f;
        g.setColour(fillColour);
        g.fillEllipse(x, y, width, height);
        g.setColour(strokeColour);
        g.drawEllipse(x, y, width, height, 3.500f);
    }
    //[/UserPaint]
}

void Gate::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void Gate::connectionMade(TargetConnector* connector, ConnectionID source_id)
{
    if(connector == target0.get())
    {
        synth->setGateInput(id, source_id, 0);
    }
    else if(connector == target1.get() && type != GateType::NOT)
    {
        synth->setGateInput(id, source_id, 1);
    }

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

