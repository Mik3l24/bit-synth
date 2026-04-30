//
// Created by micha on 24.11.2023.
//

#include "ElementAdderButton.h"

#include "DragSourceType.h"

namespace ui
{
ElementAdderButton::ElementAdderButton(ElementCategory element_type)
    : element_type(element_type), gate_type(ElementType::NONE)
{
    // Setting text is currently done in the constructor of the parent component
}

ElementAdderButton::ElementAdderButton(ElementType gate_type)
    : element_type(ElementCategory::PROCESSOR), gate_type(gate_type)
{

}

ElementAdderButton::~ElementAdderButton() = default;


ElementCategory ElementAdderButton::getElementType() const
{
    return element_type;
}

ElementType ElementAdderButton::getGateType() const
{
    return gate_type;
}


void ElementAdderButton::mouseDrag(const juce::MouseEvent& event)
{
    Button::mouseDrag(event);
    // Start the drag and drop
    juce::DragAndDropContainer::findParentDragContainerFor(this)
        ->startDragging(DragSourceType::ELEMENT_ADDER_BUTTON, this);
}




} // ui