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
//[/Headers]

#include "TargetConnector.h"
#include "StructureEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
namespace ui {
//[/MiscUserDefs]

//==============================================================================
TargetConnector::TargetConnector (ConnectionID)
{
    //[Constructor_pre] You can add your own custom stuff here..
    setButtonText(">");
    //[/Constructor_pre]


    //[UserPreSize]
    //[/UserPreSize]

    setSize (10, 10);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

TargetConnector::~TargetConnector()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void TargetConnector::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff101050));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void TargetConnector::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}


//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...

bool TargetConnector::isInterestedInDragSource(const juce::DragAndDropTarget::SourceDetails& dragSourceDetails)
{
    return dragSourceDetails.description.equals(DragSourceType::SOURCE_CONNECTOR);
}

void TargetConnector::itemDropped(const juce::DragAndDropTarget::SourceDetails& dragSourceDetails)
{
    auto* source = dynamic_cast<SourceConnector*>(dragSourceDetails.sourceComponent.get());
    if(source == nullptr)
        return;

    makeConnection(source);
}

void TargetConnector::addListener(TargetConnector::Listener* listener)
{
    connectorListeners.add(listener);
}

void TargetConnector::removeListener(TargetConnector::Listener* listener)
{
    connectorListeners.remove(listener);
}

void TargetConnector::makeConnection(SourceConnector* source)
{
    this->source = source;
    connectorListeners.call(&Listener::connectionMade, this, source->getConnectionID());

    if(auto* parent = findParentComponentOfClass<StructureEditor>())
        parent->repaint();
}

std::optional<std::pair<juce::Point<int>, juce::Point<int>>> TargetConnector::getConnectionPoints() const
{
    if(source == nullptr)
        return std::nullopt;

    return std::make_pair(getScreenPosition().translated(getWidth()/2, getHeight()/2),
                          source->getScreenPosition().translated(source->getWidth()/2, source->getHeight()/2));
}

//[/MiscUserCode]


void TargetConnector::mouseDrag(const juce::MouseEvent& e)
{
    if(e.mouseWasDraggedSinceMouseDown())
        juce::DragAndDropContainer::findParentDragContainerFor(this)
            ->startDragging(DragSourceType::TARGET_CONNECTOR, this);
    else
        juce::TextButton::mouseDrag(e);

}


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="TargetConnector" componentName=""
                 parentClasses="public juce::Button" constructorParams="ConnectionID id"
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="1" initialWidth="10" initialHeight="10">
  <BACKGROUND backgroundColour="ff505050"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
}
//[/EndFile]

