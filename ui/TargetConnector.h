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

namespace ui {
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class TargetConnector  : public juce::TextButton, public juce::DragAndDropTarget
{
public:
    class Listener
    {
    public:
        virtual ~Listener() = default;
        virtual void connectionMade(TargetConnector* connector, ConnectionID source_id) = 0;
    };
public:
    //==============================================================================
    TargetConnector (ConnectionID id);
    ~TargetConnector() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    bool isInterestedInDragSource(const juce::DragAndDropTarget::SourceDetails &dragSourceDetails) override;
    void itemDropped(const juce::DragAndDropTarget::SourceDetails &dragSourceDetails) override;

    void addListener(Listener* listener);
    void removeListener(Listener* listener);

    void makeConnection(SourceConnector * source);

    std::optional<std::pair<juce::Point<int>, juce::Point<int>>> getConnectionPoints() const;

    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    ConnectionID id;
    juce::ListenerList<Listener> connectorListeners;
    SourceConnector* source = nullptr;
    //[/UserVariables]

    //==============================================================================


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TargetConnector)
};

//[EndFile] You can add extra defines here...
}
//[/EndFile]

