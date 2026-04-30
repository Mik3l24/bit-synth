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
    explicit ElementAdderButton(ElementCategory element_type);
    explicit ElementAdderButton(ElementType gate_type);
    ~ElementAdderButton() override;

public: // Access methods
    [[nodiscard]] ElementCategory getElementType() const;
    [[nodiscard]] ElementType getGateType() const;

public: // Overrides
    void mouseDrag(const juce::MouseEvent& event) override;

protected:
    ElementCategory element_type;
    ElementType gate_type;
};

} // ui

