#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "SynthElement.h"
#include "SourceConnector.h"


namespace ui
{

class OscillatorParameters  : public SynthElement
{
public:
    OscillatorParameters(ElementID _id, SynthStateManager _state_manager);
    ~OscillatorParameters() override;

    [[nodiscard]] double getRatio() const;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;

private: // Members
    juce::ComponentDragger dragger;

    std::unique_ptr<juce::SliderParameterAttachment> pw_attachment;
    std::unique_ptr<juce::SliderParameterAttachment> phase_attachment;
    std::unique_ptr<juce::SliderParameterAttachment> ratio_attachment;

    std::unique_ptr<juce::Slider> pw_slider;
    std::unique_ptr<juce::Label> ratio_label;
    std::unique_ptr<juce::Slider> numerator_slider; // Deprecated
    std::unique_ptr<juce::Slider> phase_slider;
    std::unique_ptr<juce::Slider> denominator_slider; // Deprecated
    std::unique_ptr<juce::Label> phase_label;
    std::unique_ptr<juce::Label> pw_label;
    std::unique_ptr<juce::Slider> transpose_slider;
    std::unique_ptr<juce::Label> transpose_label;
    std::unique_ptr<juce::Slider> fine_slider;
    std::unique_ptr<juce::Label> fine_label;
    std::unique_ptr<juce::Slider> ratio_slider;

public:
    std::unique_ptr<SourceConnector> source_connector;

private:
    juce::Path internalPath1;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscillatorParameters)
};

}


