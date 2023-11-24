//
// Created by micha on 24.11.2023.
//
#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "../synth_management/SynthManagementNames.h"

namespace ui {

class ElementAdderButton : public juce::TextButton
{
public:
    explicit ElementAdderButton(ElementType element_type);
    explicit ElementAdderButton(GateType gate_type);
    ~ElementAdderButton() override;

public: // Methods
    void mouseDrag(const juce::MouseEvent& event) override;

protected:
    ElementType element_type;
    GateType gate_type;
};

} // ui

