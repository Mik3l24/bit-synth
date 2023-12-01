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
#include "synth_management/SynthManagementNames.h"
#include "DragSourceType.h"
//[/Headers]

#include "SourceConnector.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
namespace ui {
//[/MiscUserDefs]

//==============================================================================
SourceConnector::SourceConnector (ConnectionID id)
    : juce::TextButton(), id(id)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]


    //[UserPreSize]
    //[/UserPreSize]

    setSize (10, 10);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

SourceConnector::~SourceConnector()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void SourceConnector::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff505050));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void SourceConnector::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void SourceConnector::mouseDrag (const juce::MouseEvent& e)
{
    //[UserCode_mouseDrag] -- Add your code here...
    juce::DragAndDropContainer::findParentDragContainerFor(this)->startDragging(DragSourceType::SOURCE_CONNECTOR, this);
    //[/UserCode_mouseDrag]
}

void SourceConnector::mouseUp (const juce::MouseEvent& e)
{
    //[UserCode_mouseUp] -- Add your code here...
    //[/UserCode_mouseUp]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
ConnectionID SourceConnector::getConnectionID() const
{
    return id;
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="SourceConnector" componentName=""
                 parentClasses="public juce::Button" constructorParams="ConnectionID id"
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="1" initialWidth="10" initialHeight="10">
  <METHODS>
    <METHOD name="mouseDrag (const juce::MouseEvent&amp; e)"/>
    <METHOD name="mouseUp (const juce::MouseEvent&amp; e)"/>
  </METHODS>
  <BACKGROUND backgroundColour="ff505050"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
}
//[/EndFile]

