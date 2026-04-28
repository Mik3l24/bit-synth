#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "synth_management/SynthStateManager.h"
#include "SynthElement.h"
#include "TargetConnector.h"

namespace ui {

class MixChannelParameters: public SynthElement,
                            public TargetConnector::Listener
{
public:
    MixChannelParameters(ElementID _id, SynthStateManager _state_manager);
    ~MixChannelParameters() override;

    void connectionMade(TargetConnector* connector, ConnectionID source_id) override;

    std::optional<std::pair<juce::Point<int>, juce::Point<int>>> getConnectionPoints() const;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;

private:
    std::unique_ptr<juce::SliderParameterAttachment> level_attachment;
    std::unique_ptr<juce::Slider> level_slider;
    std::unique_ptr<juce::Label> level_label;
public:
    std::unique_ptr<TargetConnector> target;
private:
    juce::ComponentDragger dragger;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MixChannelParameters)
};

}

