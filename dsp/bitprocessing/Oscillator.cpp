//
// Created by micha on 06.11.2023.
//

#include "Oscillator.h"

#include "juce_graphics/fonts/harfbuzz/hb.hh"

using namespace dsp;

void Oscillator::prepareToPlay(double _sample_rate, int _samples_per_block)
{
    sample_rate = _sample_rate;
    samples_per_block = _samples_per_block;
    jassert(samples_per_block % samples_in_bitblock == 0);
    bitblocks_per_block = samples_per_block / samples_in_bitblock;
    BitSource::prepareToPlay(sample_rate, samples_per_block);
}

void Oscillator::prepareVoice(double pitch)
{
    const double samples_per_cycle_f = sample_rate / (pitch * ratio);
    // Note - this causes the frequency to be strictly quantized to the discrete
    // spectrum allowed by the sample rate. When floats are used, they at least allow
    // for the wave's actual frequency to proportionally switch between the 2 closest
    // discrete frequencies when it's not an exact match.
    samples_per_cycle = uint(samples_per_cycle_f);
    cur_sample_in_cycle = uint(starting_phase * samples_per_cycle_f);
    number_of_ones = uint(samples_per_cycle_f * pulse_width);
    number_of_zeroes = samples_per_cycle - number_of_ones;
    in_ones = cur_sample_in_cycle <= number_of_ones;

    if(number_of_ones < samples_in_bitblock)
        edgecase = FEW_ONES;
    else if(number_of_zeroes <= samples_in_bitblock)
        edgecase = FEW_ZEROES;
    else
        edgecase = NONE;

#ifdef DEBUG_VERBOSE
    std::cout << "samples_per_cycle: " << samples_per_cycle << std::endl;
    std::cout << "number_of_ones: " << number_of_ones << std::endl;
    std::cout << "number_of_zeroes: " << number_of_zeroes << std::endl;
#endif

#ifdef DEBUG_OSCILLATOR_VALIDATION
    _debug_should_be_one = cur_sample_in_cycle <= number_of_ones;
    _debug_ones_counter = 0; _debug_zeroes_counter = 0;
#endif
}

void Oscillator::processBlock(uint sample_n)
{
    // Note - currently the processing disregards the `starting_sample`
    // (effectively aligning the generated bit buffers to the start of the note)
    // and the `samples_n` being odd with regard to the bitblock size
    // (the number of bits actually written to the buffer is rounded up,
    //  so more is overwritten than the function is told to do).
    // This doesn't matter at the moment if the sinks account for that, but this may matter for e.g. delays.

    static_assert(std::is_unsigned<bitset::block_type>());
    // used to fill the current bitblock in the exact match case
    constexpr bitset::block_type ALL_ZEROES = 0,
                                 ALL_ONES = std::numeric_limits<bitset::block_type>::max();

    auto bitblock_iterator = out.m_bits.begin();

    while(sample_n)
    {
        const uint sample_n_to_process = std::min(sample_n, samples_in_bitblock);
        const uint new_sample_in_cycle = cur_sample_in_cycle + sample_n_to_process;

        // The `unlikely` optimizations here are put in assumption that the wave's frequency is much lower than the sample frequency.
        if(unlikely(new_sample_in_cycle >= samples_per_cycle))
        { // Handling the rising edge of a wave __--
            const uint cycle_diff = new_sample_in_cycle - samples_per_cycle;
#ifdef DEBUG_OSCILLATOR_VALIDATION
            if(cycle_diff >= samples_in_bitblock)
            {
                //Debug prints
                std::cout << "Rising edge, diff: " << cycle_diff << ", new_sample_in_cycle: " << new_sample_in_cycle
                          << ", samples_per_cycle: " << samples_per_cycle << std::endl;
                std::cout << "number_of_ones: " << number_of_ones << ", number_of_zeroes: " << number_of_zeroes << std::endl;
                std::cout << "cur_sample_in_cycle: " << cur_sample_in_cycle << std::endl;
            }
#endif
            jassert(cycle_diff < samples_in_bitblock); // If this fails we messed up in calculating how much the processing should move

            switch(edgecase)
            {
            case FEW_ONES:
                if(cycle_diff >= number_of_ones) // rising and falling edges ___[___---___]___
                {
                    // This is an edgecase - only occurs when pulse width is short enough that the `in_ones` part of the cycle is within 1 bitblock
                    *bitblock_iterator =
                        (ALL_ONES >> (samples_in_bitblock - number_of_ones)) // First we need to shorten the length of the sequence to the pulsewidth
                                  << (samples_in_bitblock - cycle_diff); // Then shift to the correct position like in the case below

                    in_ones = false; // As we've already covered the entire length of ones.
                    goto reset_phase; // To skip on `in_ones` being overriden
                }
                break;
            case FEW_ZEROES:
                if(const uint pulse_diff = new_sample_in_cycle - number_of_ones;
                    cycle_diff == 0) // new cycle aligns with the end of the bitblock, but we need a falling edge ---[---___]---
                {
                    *bitblock_iterator = ALL_ONES >> pulse_diff;

                    goto reset_phase_and_set_in_ones;
                }
                else if(pulse_diff < samples_in_bitblock && pulse_diff >= number_of_zeroes) // falling and rising edges ---[---___---]---
                {
                    // Another similar edgecase - pulse width is long enough that the `in_zeroes` part of the cycle is within 1 bitblock
                    // However, this one is slightly more complicated - as we need to create a sequence of zeros wrapped between ones
                    // Which we can only do by making sequences of ones of specific lengths
                    *bitblock_iterator =
                        ALL_ONES >> pulse_diff // First, however many ones are remaining in the previous cycle
                         | ALL_ONES << (number_of_zeroes + (samples_in_bitblock - pulse_diff)); // And then the ones after the zeroes (though we need to account for number of the remaining ones above)

                    goto reset_phase_and_set_in_ones;
                }
                break;
            case NONE:
                break;
            default:
                jassertfalse; // Invalid value in enum
                break;
            }

            if(cycle_diff == 0) // Easy case, the end of wave and the bitblock align ___[____]----
                *bitblock_iterator = ALL_ZEROES;
            else // rising edge ___[___---]---
                // We need to create an edge within a bitblock, that has `diff` ones.
                // Since `dynamic_bitset` counts bits from the LSB, we counterintuitively need to use an lshift.
                // Also in this case, to get the shift n we need to subtract from the number of bits in the bitblock as shifts create zeroes.
                *bitblock_iterator = ALL_ONES << (samples_in_bitblock - cycle_diff);

        reset_phase_and_set_in_ones:
            in_ones = true;
        reset_phase:
            cur_sample_in_cycle = cycle_diff; // To reset the cycle while keeping the phase
        }
        else if(unlikely(new_sample_in_cycle >= number_of_ones && in_ones))
        { // Handling the falling edge of a wave --__
            const uint pulse_diff = new_sample_in_cycle - number_of_ones;
#ifdef DEBUG_OSCILLATOR_VALIDATION
            if(pulse_diff >= samples_in_bitblock)
            {
                //Debug prints
                std::cout << "Rising edge, diff: " << pulse_diff << ", new_sample_in_cycle: " << new_sample_in_cycle
                          << ", samples_per_cycle: " << samples_per_cycle << std::endl;
                std::cout << "number_of_ones: " << number_of_ones << ", number_of_zeroes: " << number_of_zeroes << std::endl;
                std::cout << "cur_sample_in_cycle: " << cur_sample_in_cycle << std::endl;
            }
#endif
            jassert(pulse_diff < samples_in_bitblock); // If this fails we messed up in calculating how much the processing should move

            // We need to create an edge within a bitblock, that has `diff` zeroes.
            // Analogous to the new cycle case, but the diff actually tells us how many zeroes we need now.
            // Also, we do not need to check for `diff == 0` as it's going to result in no shift anyway.
            *bitblock_iterator = ALL_ONES >> pulse_diff;

            in_ones = false;
            cur_sample_in_cycle = new_sample_in_cycle;

        }
        else
        { // Carrying on in the current state
            *bitblock_iterator = in_ones ? ALL_ONES : ALL_ZEROES;

            cur_sample_in_cycle = new_sample_in_cycle;
        }

#ifdef DEBUG_OSCILLATOR_VALIDATION
        for(uint i = 0; i < sample_n_to_process; i++)
        {
            const bitset::block_type masked = *bitblock_iterator & (1 << i);
            if(masked)
            {
                if(!_debug_should_be_one)
                {
                    __debugbreak();
                }
                _debug_ones_counter++;
                if(_debug_ones_counter == number_of_ones)
                {
                    _debug_should_be_one = false;
                    _debug_zeroes_counter = 0;
                }
            }
            else
            {
                if(_debug_should_be_one)
                {
                    __debugbreak();
                }
                _debug_zeroes_counter++;
                if(_debug_zeroes_counter == number_of_zeroes)
                {
                    _debug_should_be_one = true;
                    _debug_ones_counter = 0;
                }
            }
        }
#endif

        // Move forward in processing
        sample_n -= sample_n_to_process;
        ++bitblock_iterator;
    }

}

