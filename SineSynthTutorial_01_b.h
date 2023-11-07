/*
  ==============================================================================

   This file is part of the JUCE tutorials.
   Copyright (c) 2020 - Raw Material Software Limited

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   To use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES,
   WHETHER EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR
   PURPOSE, ARE DISCLAIMED.

  ==============================================================================
*/

/*******************************************************************************
 The block below describes the properties of this PIP. A PIP is a short snippet
 of code that can be read by the Projucer and used to generate a JUCE project.

 BEGIN_JUCE_PIP_METADATA

 name:             SineSynthTutorial
 version:          1.0.0
 vendor:           JUCE
 website:          http://juce.com
 description:      Simple sine synthesiser.

 dependencies:     juce_audio_basics, juce_audio_devices, juce_audio_formats,
                   juce_audio_processors, juce_audio_utils, juce_core,
                   juce_data_structures, juce_events, juce_graphics,
                   juce_gui_basics, juce_gui_extra
 exporters:        xcode_mac, vs2019, linux_make

 type:             Component
 mainClass:        MainContentComponent

 useLocalCopy:     1

 END_JUCE_PIP_METADATA

*******************************************************************************/


#pragma once

#include <boost/dynamic_bitset.hpp>

//==============================================================================
class MainContentComponent   : public juce::AudioAppComponent
{
public:
    MainContentComponent()
    {
        addAndMakeVisible (frequencySliderOsc1);
        frequencySliderOsc1.setRange (50.0, 5000.0);
        frequencySliderOsc1.setSkewFactorFromMidPoint (500.0); // [4]
        frequencySliderOsc1.onValueChange = [this]
        {
            if (currentSampleRate > 0.0)
                updateAngleDelta();
        };
        addAndMakeVisible (frequencySliderOsc2);
        frequencySliderOsc2.setRange (50.0, 5000.0);
        frequencySliderOsc2.setSkewFactorFromMidPoint (500.0); // [4]
        frequencySliderOsc2.onValueChange = [this]
        {
            if (currentSampleRate > 0.0)
                updateAngleDelta();
        };
        

        setSize (600, 100);
        setAudioChannels (0, 2); // no inputs, two outputs
    }

    ~MainContentComponent() override
    {
        shutdownAudio();
    }

    void resized() override
    {
        frequencySliderOsc1.setBounds (10, 10, getWidth() - 20, 20);
        frequencySliderOsc2.setBounds (10, 40, getWidth() - 20, 20);
    }

    void updateAngleDelta()
    {
        auto cyclesPerSampleOsc1 = frequencySliderOsc1.getValue() / currentSampleRate;
        angleDeltaOsc1 = cyclesPerSampleOsc1 * 2.0 * juce::MathConstants<double>::pi;

        auto cyclesPerSampleOsc2 = frequencySliderOsc2.getValue() / currentSampleRate;
        angleDeltaOsc2 = cyclesPerSampleOsc2 * 2.0 * juce::MathConstants<double>::pi;
    }

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        currentSampleRate = sampleRate;
        samplesPerBlock = samplesPerBlockExpected;
        bitBuffer = boost::dynamic_bitset<>(samplesPerBlock);
        osc1 = boost::dynamic_bitset<>(samplesPerBlock);
        osc2 = boost::dynamic_bitset<>(samplesPerBlock);
        updateAngleDelta();
    }

    void releaseResources() override {}

    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override
    {
        auto level = 0.125f;
        auto* leftBuffer  = bufferToFill.buffer->getWritePointer (0, bufferToFill.startSample);
        auto* rightBuffer = bufferToFill.buffer->getWritePointer (1, bufferToFill.startSample);

        // bit operation on previous buffer
        bitBuffer = osc1 & osc2;

        for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
        {
            osc1[sample] = currentAngleOsc1 < juce::MathConstants<double>::pi;
            osc2[sample] = currentAngleOsc2 < juce::MathConstants<double>::pi;

            currentAngleOsc1 += angleDeltaOsc1;
            if(currentAngleOsc1 > juce::MathConstants<double>::twoPi)
                currentAngleOsc1 = 0;
            currentAngleOsc2 += angleDeltaOsc2;
            if(currentAngleOsc2 > juce::MathConstants<double>::twoPi)
                currentAngleOsc2 = 0;

            auto currentSample = bitBuffer[sample] ? level : -level;
            leftBuffer[sample]  = currentSample;
            rightBuffer[sample] = currentSample;
        }
    }

private:
    juce::Slider frequencySliderOsc1, frequencySliderOsc2;
    double currentSampleRate = 0.0, currentAngleOsc1 = 0.0, angleDeltaOsc1 = 0.0, currentAngleOsc2 = 0.0, angleDeltaOsc2 = 0.0;
    int samplesPerBlock = 0;
    boost::dynamic_bitset<> bitBuffer, osc1, osc2;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};
