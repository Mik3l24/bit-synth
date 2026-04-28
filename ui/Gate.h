#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "synth_management/SynthManagementNames.h"
#include "synth_management/SynthStateManager.h"

#include "SynthElement.h"
#include "SourceConnector.h"
#include "TargetConnector.h"

namespace ui {

class Gate:
    public SynthElement,
    public TargetConnector::Listener
{
public:
    Gate(ElementID _id, GateType _type, SynthStateManager _state_manager);
    ~Gate() override;

    void connectionMade(TargetConnector* connector, ConnectionID source_id) override;

    [[nodiscard]] std::optional<std::pair<juce::Point<int>, juce::Point<int>>> getConnectionPoints(unsigned int target) const;


    void paint(juce::Graphics& g) override;
    void resized() override;

    void mouseDown(const juce::MouseEvent &e) override;
    void mouseDrag(const juce::MouseEvent &e) override;
    void mouseUp(const juce::MouseEvent &e) override;


public: // Members
    const GateType type = GateType::NONE;

public:
    std::unique_ptr<SourceConnector> source;
    std::unique_ptr<TargetConnector> target0;
    std::unique_ptr<TargetConnector> target1;

private:
    juce::Path bodyPath;
    juce::Path connectorsPath;

    juce::ComponentDragger dragger;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Gate)
};

}
