#pragma once

//[Headers]     -- You can add your own extra header files here --
#include <juce_gui_basics/juce_gui_basics.h>

#include "synth_management/SynthManagementNames.h"
#include "SourceConnector.h"


namespace ui {

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
    explicit TargetConnector (ConnectionID id);
    ~TargetConnector() override;

    bool isInterestedInDragSource(const juce::DragAndDropTarget::SourceDetails &dragSourceDetails) override;
    void itemDropped(const juce::DragAndDropTarget::SourceDetails &dragSourceDetails) override;

    void addListener(Listener* listener);
    void removeListener(Listener* listener);

    void makeConnection(SourceConnector* source, bool update = true);

    [[nodiscard]] std::optional<std::pair<juce::Point<int>, juce::Point<int>>> getConnectionPoints() const;

    void paint (juce::Graphics& g) override;
    void resized() override;
    void mouseDrag(const juce::MouseEvent& e) override;

public:
    const ConnectionID id;

private:
    juce::ListenerList<Listener> connectorListeners;
    SourceConnector* source = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TargetConnector)
};

}

