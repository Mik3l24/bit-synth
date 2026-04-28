#include "DragSourceType.h"
//[/Headers]

#include "TargetConnector.h"
#include "StructureEditor.h"
#include "juce_graphics/fonts/harfbuzz/hb.hh"


namespace ui {



TargetConnector::TargetConnector(ConnectionID id)
    : id(id)
{
    setButtonText(">");

    setSize (10, 10);
}

TargetConnector::~TargetConnector() = default;


void TargetConnector::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff101050));
}

void TargetConnector::resized() {}


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

void TargetConnector::makeConnection(SourceConnector* source, const bool update)
{
    this->source = source;
    if(likely(update)) // Will only be false when remaking the editor ui
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



void TargetConnector::mouseDrag(const juce::MouseEvent& e)
{
    if(e.mouseWasDraggedSinceMouseDown())
        juce::DragAndDropContainer::findParentDragContainerFor(this)
            ->startDragging(DragSourceType::TARGET_CONNECTOR, this);
    else
        juce::TextButton::mouseDrag(e);

}

}
