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
#include "../synth_management/SynthConnected.h"

#include "SourceConnector.h"
#include "TargetConnector.h"

namespace ui {
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class Gate:
    public juce::Component,
    public SynthConnected,
    public TargetConnector::Listener
{
public:
    //==============================================================================
    Gate(ConnectionID id, GateType type, BitSynthesizer* synth);
    ~Gate() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void connectionMade(TargetConnector* connector, ConnectionID source_id) override;

    [[nodiscard]] std::optional<std::pair<juce::Point<int>, juce::Point<int>>> getConnectionPoints(unsigned int target) const;

    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;

    void mouseDown(const juce::MouseEvent &e) override;
    void mouseDrag(const juce::MouseEvent &e) override;

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    GateType type = GateType::NONE;
    ConnectionID id;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<SourceConnector> source;
    std::unique_ptr<TargetConnector> target0;
    std::unique_ptr<TargetConnector> target1;
    juce::Path bodyPath;
    juce::Path connectorsPath;

    juce::ComponentDragger dragger;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Gate)
};

//[EndFile] You can add extra defines here...
}
//[/EndFile]

