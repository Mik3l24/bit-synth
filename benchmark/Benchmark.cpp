//
// Created by mik3l on 7.05.2026.
//

#include "Benchmark.h"
#include <memory>
#include "SynthAudioProcessor.h"

void runBenchmark(juce::File patch_file, juce::StringRef name, BenchmarkSettings settings, juce::File logging_file)
{
    std::clog << "--- Running benchmark ---" << std::endl;
    std::clog << "Patch: " << patch_file.getFileNameWithoutExtension() << std::endl;
    std::clog << "Settings: " << settings << std::endl;

    const auto [sample_rate, channel_n, block_size, note, runs_n] = settings;

    const auto audio_processor = std::make_unique<SynthAudioProcessor>();
    jassert(audio_processor != nullptr);
    jassert(audio_processor->acceptsMidi());

    // Load test patch
    {
        jassert(patch_file.existsAsFile());
        const auto xml = juce::XmlDocument::parse(patch_file);
        jassert(xml != nullptr && xml->hasTagName(Name::ROOT));
        audio_processor->setStateFromXml(*xml);
    }
    audio_processor->prepareToPlay(sample_rate, block_size);

    const auto performance_counter = std::make_unique<juce::PerformanceCounter>(
        name + "_" + patch_file.getFileNameWithoutExtension(), runs_n, logging_file
    );
    jassert(performance_counter != nullptr);

    auto block = juce::AudioSampleBuffer(channel_n, block_size);
    auto starting_midi_buffer = juce::MidiBuffer(juce::MidiMessage::noteOn(1, note, juce::uint8(127)));
    auto empty_midi_buffer = juce::MidiBuffer();

    // Make a single empty run to activate the note
    audio_processor->processBlock(block, starting_midi_buffer);

    // Benchmark!
    for(auto i = 0; i < runs_n; i++)
    {
        performance_counter->start();
        audio_processor->processBlock(block, empty_midi_buffer);
        performance_counter->stop();
    }
}
