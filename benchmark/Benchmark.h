//
// Created by mik3l on 7.05.2026.
//

#pragma once

#include <juce_core/juce_core.h>
#include "Utils.h"

struct BenchmarkSettings
{
    int sample_rate = 44100;
    int channel_n = 2;
    int block_size = 512;
    int total_samples = block_size;
    int note = getMidiNoteNumberFromName("C4");
    int runs_n = 256;

    explicit operator juce::String() const
    {
        return "{ "
               "sample_rate=" + juce::String(sample_rate) + ", " +
               "channel_n=" + juce::String(channel_n) + ", " +
               "block_size=" + juce::String(block_size) + ", " +
               "note=" + juce::String(note) + ", " +
               "runs_n=" + juce::String(runs_n) + ", " +
               "total_samples=" + juce::String(total_samples) +
               " }";
    }
};

template<class traits>
std::basic_ostream<char, traits>& operator<<(std::basic_ostream <char, traits>& stream, const BenchmarkSettings& settings)
{
    return stream << juce::String(settings);
}

void runBenchmark(
    juce::File patch_file,
    juce::StringRef name = "BitBenchmark",
    BenchmarkSettings settings = BenchmarkSettings(),
    juce::File logging_file = juce::File()
);


