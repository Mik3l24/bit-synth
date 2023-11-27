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

#include "SourceConnector.h"
#include "../synth_management/SynthConnected.h"


namespace ui {
//[/Headers]




//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class OscillatorParameters  : public juce::Component,
                              public juce::Slider::Listener,
                              public SynthConnected
{
public:
    //==============================================================================
    OscillatorParameters(ConnectionID id, BitSynthesizer* synth);
    ~OscillatorParameters() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void sliderValueChanged (juce::Slider* sliderThatWasMoved) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    const ConnectionID id;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::Slider> pw_slider;
    std::unique_ptr<juce::Label> ratio_label;
    std::unique_ptr<juce::Slider> numerator_slider;
    std::unique_ptr<juce::Slider> phase_slider;
    std::unique_ptr<juce::Slider> denominator_slider;
    std::unique_ptr<juce::Label> phase_label;
    std::unique_ptr<juce::Label> pw_label;
    std::unique_ptr<juce::Slider> transpose_slider;
    std::unique_ptr<juce::Label> transpose_label;
    std::unique_ptr<juce::Slider> fine_slider;
    std::unique_ptr<juce::Label> fine_label;
    std::unique_ptr<SourceConnector> source_connector;
    juce::Path internalPath1;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscillatorParameters)
};

//[EndFile] You can add extra defines here...
}
//[/EndFile]

