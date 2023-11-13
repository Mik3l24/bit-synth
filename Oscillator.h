//
// Created by micha on 06.11.2023.
//
#pragma once
#include "BitIO.h"
#include <juce_core/juce_core.h>

class Oscillator : public BitSource
{
public:
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void prepareVoice(double pitch);
    void processSample(int sample_index);

public: // Constructors, destructors
    // Oscillators are all processed first, so they're always ready
    Oscillator() { ready = true; }

public:
    void setRatio(double _ratio) { ratio = _ratio; }
    void setPulseWidth(float _pulse_width) { pulse_width = _pulse_width; }
    void setStartingPhase(double _starting_phase) { starting_phase = _starting_phase; }

protected: // Parameters
    double ratio = 1.0; // Ratio of oscillator's pitch to note's pitch
    float pulse_width = 0.5;
    double starting_phase = 0.0;

private: // Internal processing variables
    double sample_rate = 44100.0;
    int samples_per_block = 512;

    double current_angle = 0.0, angle_delta = 0.0;
    double width_threshold = juce::MathConstants<double>::pi;
};

