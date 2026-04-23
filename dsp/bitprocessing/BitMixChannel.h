//
// Created by micha on 07.11.2023.
//
#pragma once
#include "BitIO.h"
#include <atomic>


// Always one input
class BitMixChannel : public BitReceiver
{
public:
    explicit BitMixChannel(std::atomic<float>& _level)
        : BitReceiver(1), level(_level) {}

public:
    float getSample(int sample_index);

    [[deprecated]] void setLevel(float _level) { level = _level; }

protected: // Parameters
    std::atomic<float>& level;
};



