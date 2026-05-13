//
// Created by micha on 07.11.2023.
//
#pragma once
#include "BitIO.h"
#include <atomic>

namespace dsp
{

// Always one input
class BitMixChannel : public BitReceiver
{
public:
    explicit BitMixChannel(const bitset& _zeroes, std::atomic<float>& _level)
        : BitReceiver(_zeroes, 1), level(_level) {}

public:
    float getSample(int sample_index);

    [[deprecated]] void setLevel(float _level) { level = _level; }

protected: // Parameters
    std::atomic<float>& level;
};

}
