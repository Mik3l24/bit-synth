//
// Created by mik3l on 28.04.2026.
//
#include "SynthElement.h"

#include "StructureEditor.h"

namespace ui
{

void SynthElement::snapAndSavePosition()
{
    constexpr int quantisation = 2;
    auto position = getPosition();
    position.x = position.x >> quantisation << quantisation;
    position.y = position.y >> quantisation << quantisation;
    setTopLeftPosition(position);
    state_manager.setElementPosition(id, category, position);
    repaintEditor();
}

void SynthElement::repaintEditor() const
{
    if(auto* parent = findParentComponentOfClass<StructureEditor>())
        parent->repaint();
}

}
