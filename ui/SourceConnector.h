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

#include "../synth_management/SynthManagementNames.h"

namespace ui {
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class SourceConnector  : public juce::TextButton
{
public:
    //==============================================================================
    SourceConnector (ConnectionID id);
    ~SourceConnector() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    ConnectionID getConnectionID() const;
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void mouseDrag (const juce::MouseEvent& e) override;
    void mouseUp (const juce::MouseEvent& e) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    ConnectionID id;
    //[/UserVariables]

    //==============================================================================


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SourceConnector)
};

//[EndFile] You can add extra defines here...
}
//[/EndFile]

