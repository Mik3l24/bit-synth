//
// Created by micha on 06.11.2023.
//

#include "Oscillator.h"


void Oscillator::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    sample_rate = sampleRate;
    samples_per_block = samplesPerBlockExpected;
    out = bitset(samples_per_block);
}

void Oscillator::prepareVoice(double pitch)
{
    // Handling of parameters changing mid-block can be implemented later if need-be
    angle_delta = pitch * ratio
            * juce::MathConstants<double>::twoPi / sample_rate;
    current_angle = starting_phase;
    // TODO: Assert between 0 and 1
    width_threshold = pulse_width * juce::MathConstants<double>::twoPi;
}

void Oscillator::processSample(int sample_index)
{
    out[sample_index] = current_angle < width_threshold;

    current_angle += angle_delta;
    if(current_angle > juce::MathConstants<double>::twoPi)
        current_angle -= juce::MathConstants<double>::twoPi;
}
