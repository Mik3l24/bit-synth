#include "synth_management/SynthManagementNames.h"
#include "DragSourceType.h"
#include "TargetConnector.h"
//[/Headers]

#include "SourceConnector.h"

namespace ui {

SourceConnector::SourceConnector (ConnectionID id)
    : juce::TextButton(), id(id)
{
    setSize (10, 10);
}

SourceConnector::~SourceConnector() = default;


void SourceConnector::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff505050));
}

void SourceConnector::resized() {}

void SourceConnector::mouseDrag (const juce::MouseEvent&)
{
    juce::DragAndDropContainer::findParentDragContainerFor(this)->startDragging(DragSourceType::SOURCE_CONNECTOR, this);
}

void SourceConnector::mouseUp (const juce::MouseEvent&) {}


bool SourceConnector::isInterestedInDragSource(const juce::DragAndDropTarget::SourceDetails& dragSourceDetails)
{
    return dragSourceDetails.description.equals(DragSourceType::TARGET_CONNECTOR);
}

void SourceConnector::itemDropped(const juce::DragAndDropTarget::SourceDetails& dragSourceDetails)
{
    auto* target = dynamic_cast<TargetConnector*>(dragSourceDetails.sourceComponent.get());
    if(target == nullptr)
        return;

    target->makeConnection(this);
}

ConnectionID SourceConnector::getConnectionID() const
{
    return id;
}

}
