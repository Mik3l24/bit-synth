#include <iostream>
#include <memory>
#include <juce_gui_basics/juce_gui_basics.h>

#include "Benchmark.h"

// Some form of a JUCE application is required for parts of SynthAudioProcessor to work without warnings.
class BenchmarkApplication final : public juce::JUCEApplication
{
public:
    BenchmarkApplication() = default;

    const juce::String getApplicationName() override       { return JUCE_APPLICATION_NAME_STRING; }
    const juce::String getApplicationVersion() override    { return JUCE_APPLICATION_VERSION_STRING; }
    bool moreThanOneInstanceAllowed() override             { return true; }

    void initialise(const juce::String& commandLine) override
    {
#ifdef DEBUG_VERBOSE
        std::clog << "Application::initialise()" << std::endl;
        std::clog << "Command line: `" << commandLine << '`' << std::endl;
#endif

        const auto args = juce::StringArray::fromTokens(commandLine, true);
        if(args.size() < 1)
        {
            std::clog << "Expected at least one argument.\n"
                         "Usage: bitprocessing_benchmark <patch_file_dir>" << std::endl;
            exit(EXIT_FAILURE);
        }

        const auto input = juce::File(args[0]);
        if(!input.exists())
        {
            std::clog << "Input path does not exist: " << input.getFullPathName() << std::endl;
            exit(EXIT_FAILURE);
        }

        if(input.isDirectory())
        {
            std::clog << "Input is a directory. Running benchmarks for all patches in the directory: " << input.getFullPathName() << std::endl;

            // Queue the benchmark to run after the MessageManager starts the event loop
            const bool ret = juce::MessageManager::getInstance()->callAsync(
            [input]
            {
                const auto& dir = input;
                jassert(dir.exists() && dir.isDirectory());
                juce::uint32 test_count = 0;
                for(const auto& patch : dir.findChildFiles(juce::File::findFiles, false, "*.xml"))
                {
                    std::clog << "--- " << test_count++ << " ---" << std::endl;
                    runBenchmark(patch);
                }
                quit();
            });
            jassert(ret);
        }
        else // Only for one file
        {
            const bool ret = juce::MessageManager::getInstance()->callAsync(
            [input]
            {
                jassert(input.existsAsFile());
                runBenchmark(input);
                quit();
            });
            jassert(ret);
        }
    }

    void shutdown() override
    {
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        // This is called when the app is being asked to quit: you can ignore this
        // request and let the app carry on running, or call quit() to allow the app to close.
        quit();
    }

    void anotherInstanceStarted (const juce::String& commandLine) override
    {
        // When another instance of the app is launched while this one is running,
        // this method is invoked, and the commandLine parameter tells you what
        // the other instance's command-line arguments were.
        juce::ignoreUnused (commandLine);
    }

};

START_JUCE_APPLICATION(BenchmarkApplication)
