//
// Created by micha on 06.11.2023.
//
#pragma once
#include "BitIO.h"
#include <juce_core/juce_core.h>
#include <atomic>

class Oscillator : public BitSource
{
public: // Constructors, destructors
    // Oscillators are all processed first, so they're always ready
    Oscillator(std::atomic<float>& _ratio, std::atomic<float>& _pulse_width, std::atomic<float>& _starting_phase)
        : ratio(_ratio), pulse_width(_pulse_width), starting_phase(_starting_phase) { ready = true; }

public: // DSP setup and processing methods
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void prepareVoice(double pitch);
    void processSample(int sample_index);

public: // Parameter setters, getters
    [[deprecated]] void setRatio(float _ratio) { ratio = _ratio; }
    [[deprecated]] void setPulseWidth(float _pulse_width) { pulse_width = _pulse_width; }
    [[deprecated]] void setStartingPhase(float _starting_phase) { starting_phase = _starting_phase; }

protected: // Parameters
    std::atomic<float>& ratio; // Ratio of oscillator's pitch to note's pitch
    std::atomic<float>& pulse_width;
    std::atomic<float>& starting_phase;

private: // Internal processing variables
    double sample_rate = 44100.0;
    int samples_per_block = 512;

    double current_angle = 0.0, angle_delta = 0.0;
    double width_threshold = juce::MathConstants<double>::pi;
};

