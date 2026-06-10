//
// Created by micha on 06.11.2023.
//
#pragma once

namespace dsp { class Oscillator; }
#define BOOST_DYNAMIC_BITSET_MAKE_FRIEND \
    friend class dsp::Oscillator;

#include "BitIO.h"
#include <juce_core/juce_core.h>
#include <atomic>

namespace dsp
{

class Oscillator : public BitSource
{
public:
    typedef juce::uint64 uint;
public: // Constructors, destructors
    Oscillator(std::atomic<float>& _ratio, std::atomic<float>& _pulse_width, std::atomic<float>& _starting_phase)
        : ratio(_ratio), pulse_width(_pulse_width), starting_phase(_starting_phase)
    { ready = true; } // Oscillators are all processed first, so they're always ready

public: // DSP setup and processing methods
    void prepareToPlay(double _sample_rate, int _samples_per_block) override;
    void prepareVoice(double pitch);
    void processBlock(uint sample_n); // There used to be `starting_sample` argument, but got removed for simplicity

public: // Parameter setters, getters
    [[deprecated]] void setRatio(float _ratio) { ratio = _ratio; }
    [[deprecated]] void setPulseWidth(float _pulse_width) { pulse_width = _pulse_width; }
    [[deprecated]] void setStartingPhase(float _starting_phase) { starting_phase = _starting_phase; }

protected: // Parameters
    std::atomic<float>& ratio; // Ratio of oscillator's pitch to note's pitch
    std::atomic<float>& pulse_width;
    std::atomic<float>& starting_phase;

private: // Internal processing variables
    static constexpr uint samples_in_bitblock = std::numeric_limits<bitset::block_type>::digits;
    double sample_rate = 44100.0;
    uint samples_per_block = 512, bitblocks_per_block = samples_per_block / samples_in_bitblock;

    uint samples_per_cycle = 0, number_of_ones = 0, number_of_zeroes = 0,
         cur_sample_in_cycle = 0;
    bool in_ones = true;
    enum : uint8_t { NONE, FEW_ZEROES, FEW_ONES } edgecase = NONE;

#ifdef DEBUG_OSCILLATOR_VALIDATION
    bool _debug_should_be_one = true;
    uint _debug_ones_counter = 0, _debug_zeroes_counter = 0;
#endif
};

}
