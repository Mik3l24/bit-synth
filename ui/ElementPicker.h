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

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include <juce_gui_basics/juce_gui_basics.h>

#include "ElementAdderButton.h"

namespace ui {
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class ElementPicker  : public juce::Component,
                       public juce::Button::Listener
{
public:
    //==============================================================================
    ElementPicker ();
    ~ElementPicker() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<ElementAdderButton> osc_button;
    std::unique_ptr<ElementAdderButton> not_button;
    std::unique_ptr<ElementAdderButton> and_button;
    std::unique_ptr<ElementAdderButton> or_button;
    std::unique_ptr<ElementAdderButton> xor_button;
    std::unique_ptr<ElementAdderButton> mix_button;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ElementPicker)
};

//[EndFile] You can add extra defines here...
}
//[/EndFile]

