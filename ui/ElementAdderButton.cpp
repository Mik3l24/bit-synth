//
// Created by micha on 24.11.2023.
//

#include "ElementAdderButton.h"

namespace ui
{
ElementAdderButton::ElementAdderButton(ElementType element_type)
    : element_type(element_type), gate_type(GateType::NONE)
{
    // Setting text is currently done in the constructor of the parent component
}

ElementAdderButton::ElementAdderButton(GateType gate_type)
    : element_type(ElementType::GATE), gate_type(gate_type)
{

}

void ElementAdderButton::mouseDrag(const juce::MouseEvent& event)
{
    Button::mouseDrag(event);
    // Start the drag and drop
//    juce::DragAndDropContainer::findParentDragContainerFor(this)
//        ->startDragging("", this);
}

ElementAdderButton::~ElementAdderButton() = default;


} // ui