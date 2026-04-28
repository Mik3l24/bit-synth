#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "synth_management/SynthManagementNames.h"

namespace ui {

class SourceConnector  : public juce::TextButton, public juce::DragAndDropTarget
{
public:
    explicit SourceConnector (ConnectionID id);
    ~SourceConnector() override;

    [[nodiscard]] ConnectionID getConnectionID() const;


protected:
    void paint (juce::Graphics& g) override;
    void mouseDrag (const juce::MouseEvent& e) override;
    void mouseUp (const juce::MouseEvent& e) override;

public:
    void resized() override;
    bool isInterestedInDragSource(const juce::DragAndDropTarget::SourceDetails &dragSourceDetails) override;
    void itemDropped(const juce::DragAndDropTarget::SourceDetails &dragSourceDetails) override;

private:
    ConnectionID id;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SourceConnector)
};

}


