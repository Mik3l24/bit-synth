//
// Created by mik3l on 28.04.2026.
//

#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

#include "synth_management/SynthStateManager.h"
#include "synth_management/SynthManagementNames.h"

namespace ui
{

class SynthElement : public juce::Component
{
public:
    explicit SynthElement(const ElementID _id, const ElementCategory _category, const SynthStateManager _state_manager)
        : id(_id), category(_category), state_manager(_state_manager) {}

protected: // Methods
    void snapAndSavePosition();
    void repaintEditor() const;

public: // Members
    const ElementID id;
    const ElementCategory category;

protected: // Members
    SynthStateManager state_manager;
};




} // ui
