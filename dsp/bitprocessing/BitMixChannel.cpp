//
// Created by micha on 07.11.2023.
//
#include "BitMixChannel.h"
#include <array>

using namespace dsp;

constexpr static auto UINT8_DIGITS = std::numeric_limits<juce::uint8>::digits;
constexpr static auto UINT8_COMBINATION_N = std::numeric_limits<juce::uint8>::max() + 1;
constexpr static auto BIT_LOOKUP = []
{
    std::array<std::array<bool, UINT8_DIGITS>, UINT8_COMBINATION_N> lookup;
    for(size_t i = 0; i < UINT8_COMBINATION_N; i++)
    {
        for(size_t j = 0; j < UINT8_DIGITS; j++)
        {
            const auto masked = i & (1ull << j);
            lookup[i][j] = masked != 0;
        }
    }
    return lookup;
}();

void BitMixChannel::beginIteration()
{
    const auto& bitset_array = getOutFromInput().m_bits;
    bitblock_iterator = std::span(reinterpret_cast<const uint8_t*>(bitset_array.data()),
                                  bitset_array.size() * sizeof(bitset::block_type)).begin();
    current_bit = 0;
    cur_level = level;
}

float BitMixChannel::getSampleAndIterate()
{
    // Should be skipped if not connected, but just in case
    if(checkConnections() == status::UNCONNECTED)
        return 0.0f;

    // Currently 0 to level, could alternatively be -level/2 to level/2
    const bool bit = BIT_LOOKUP[*bitblock_iterator][current_bit];

    if(++current_bit >= UINT8_DIGITS)
    {
        current_bit = 0;
        ++bitblock_iterator;
    }

    return bit ? cur_level : -cur_level;
}

