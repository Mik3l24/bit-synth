#include <iostream>
#include <memory>
#include <juce_gui_basics/juce_gui_basics.h>

#include "Benchmark.h"

static std::unique_ptr<juce::ArgumentList> args_global;

static void setIntWithOptionalArgument(juce::StringRef option, int& to_set, juce::ArgumentList& args, const bool remove = false)
{
    const juce::String value = remove ? args.removeValueForOption(option) : args.getValueForOption(option);
    if(value.isEmpty())
        return;
    const int result = value.getIntValue();
    if(result == 0)
        std::clog << "Warning: argument `" << juce::String(option) << "` parsed as 0 - it may be not an integer" << std::endl;
    to_set = result;
}

static void setNoteWithOptionalArgument(juce::StringRef option, int& to_set, juce::ArgumentList& args, const bool remove = false)
{
    const juce::String value = remove ? args.removeValueForOption(option) : args.getValueForOption(option);
    if(value.isEmpty())
        return;
    const int result = getMidiNoteNumberFromName(value.toRawUTF8());
    if(result == -1)
    {
        std::clog << "Warning: `" << value << "` is an invalid note name." << std::endl;
        return;
    }
    to_set = result;
}


// Some form of a JUCE application is required for parts of SynthAudioProcessor to work without warnings.
class BenchmarkApplication final : public juce::JUCEApplication
{
public:
    BenchmarkApplication() = default;

    const juce::String getApplicationName() override       { return JUCE_APPLICATION_NAME_STRING; }
    const juce::String getApplicationVersion() override    { return JUCE_APPLICATION_VERSION_STRING; }
    bool moreThanOneInstanceAllowed() override             { return true; }

    void initialise(const juce::String&) override
    {
#ifdef DEBUG_VERBOSE
        std::clog << "Application::initialise()" << std::endl;
        std::clog << "Command line: `" << commandLine << '`' << std::endl;
#endif

        jassert(args_global != nullptr);
        auto& args = *args_global;
        if(args.size() < 1)
        {
            std::clog << "Expected at least one argument.\n"
                         "Usage: " << args.executableName << " [--input|-i] <patch_file_or_dir> [OPTIONS]\n"
                         "Options:\n"
                         "    --sample_rate\n"
                         "    --block_size\n"
                         "    --note\n"
                         "    --runs_n\n"
                         "    --test_name\n"
                         "    --log_file\n"
                         << std::endl;
            exit(EXIT_FAILURE);
        }

        BenchmarkSettings settings;
        setIntWithOptionalArgument( "--sample_rate", settings.sample_rate, args, true);
        setIntWithOptionalArgument( "--block_size", settings.block_size, args, true);
        setIntWithOptionalArgument("--total_samples", settings.total_samples, args, true);
        setNoteWithOptionalArgument("--note", settings.note, args, true);
        setIntWithOptionalArgument( "--runs_n", settings.runs_n, args, true);

        juce::String test_name = args.getValueForOption("--test_name");
        if(test_name.isEmpty())
            test_name = "BitBenchmark";


        juce::File log_file;
        if(const juce::String log_file_name = args.removeValueForOption("--log_file");
            log_file_name.isNotEmpty())
        {
            log_file = juce::File(log_file_name);
            if(log_file.isDirectory())
            {
                std::clog << "Warning: log file `" << log_file_name << "` is a directory. Ignoring." << std::endl;
                log_file = juce::File();
            }
            else if(!log_file.exists())
                log_file.create();
            // Otherwise file already exists, don't have to do anything with it
        }

        juce::File input;
        if(args.containsOption("--input|-i"))
        {
            input = juce::File(args.getValueForOption("--input|-i"));
        }
        else
        {
            input = juce::File(args[0].text);
        }

        if(!input.exists())
        {
            std::clog << "Input file or directory does not exist: " << input.getFullPathName() << std::endl;
            exit(EXIT_FAILURE);
        }

        if(input.isDirectory())
        {
            std::clog << "Input is a directory. Running benchmarks for all patches in the directory: " << input.getFullPathName() << std::endl;

            // Queue the benchmark to run after the MessageManager starts the event loop
            const bool ret = juce::MessageManager::callAsync(
            [input, settings, test_name, log_file]
            {
                jassert(input.exists() && input.isDirectory());
                juce::uint32 test_count = 0;
                for(const auto& patch : input.findChildFiles(juce::File::findFiles, false, "*.xml"))
                {
                    std::clog << "--- " << test_count++ << " ---" << std::endl;
                    runBenchmark(patch, test_name, settings, log_file);
                }
                quit();
            });
            jassert(ret);
        }
        else // Only for one file
        {
            const bool ret = juce::MessageManager::callAsync(
            [input, settings, test_name, log_file]
            {
                jassert(input.existsAsFile());
                runBenchmark(input, test_name, settings, log_file);
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

    void anotherInstanceStarted(const juce::String& commandLine) override
    {
        // When another instance of the app is launched while this one is running,
        // this method is invoked, and the commandLine parameter tells you what
        // the other instance's command-line arguments were.
        juce::ignoreUnused (commandLine);
    }

};

juce::JUCEApplicationBase* juce_CreateApplication();
juce::JUCEApplicationBase* juce_CreateApplication() { return new BenchmarkApplication(); }

int main(int argc, char* argv[])
{
    args_global = std::make_unique<juce::ArgumentList>(argc, argv);
    juce::JUCEApplicationBase::createInstance = &juce_CreateApplication;
    return juce::JUCEApplicationBase::main(argc, const_cast<const char**>(argv));
}
