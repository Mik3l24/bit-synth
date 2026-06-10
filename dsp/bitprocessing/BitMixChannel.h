//
// Created by micha on 07.11.2023.
//
#pragma once

// Perhaps it would be better to just declare the friends in the vendored implementation of boost::dynamic_bitset...
namespace dsp { class BitMixChannel; }
#ifndef BOOST_DYNAMIC_BITSET_MAKE_FRIEND
#define BOOST_DYNAMIC_BITSET_MAKE_FRIEND \
    friend class dsp::BitMixChannel;
#endif

#include "BitIO.h"
#include <atomic>
#include <span>

namespace dsp
{

// Always one input
class BitMixChannel : public BitReceiver
{
public:
    explicit BitMixChannel(const bitset& _zeroes, std::atomic<float>& _level)
        : BitReceiver(_zeroes, 1), level(_level) {}

public:
    void beginIteration();
    float getSampleAndIterate();

    [[deprecated]] void setLevel(float _level) { cur_level = _level; }

protected: // Parameters
    std::atomic<float>& level;
    float cur_level;

private: // Implementation members
    std::span<const uint8_t>::iterator bitblock_iterator;
    size_t current_bit = 0;
};

}
