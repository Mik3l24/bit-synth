//
// Created by mik3l on 8.05.2026.
//

#include <juce_core/juce_core.h>
#include "synth_management/SynthManagementNames.h"
#include "synth_management/SynthStateManager.h"

class StubAudioProcessor : public juce::AudioProcessor
{
public:
    const juce::String getName() const override {return {};}

    void prepareToPlay(double sampleRate, int maximumExpectedSamplesPerBlock) override {}

    void releaseResources() override {}

    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override {}

    double getTailLengthSeconds() const override { return 0; }

    bool acceptsMidi() const override { return false; }

    bool producesMidi() const override { return false; }

    juce::AudioProcessorEditor* createEditor() override { return nullptr; }

    bool hasEditor() const override { return false; }

    int getNumPrograms() override { return 0; }

    int getCurrentProgram() override { return 0; }

    void setCurrentProgram(int index) override {}

    const juce::String getProgramName(int index) override { return {}; }

    void changeProgramName(int index, const juce::String& newName) override {}

    void getStateInformation(juce::MemoryBlock& destData) override {}

    void setStateInformation(const void* data, int sizeInBytes) override {}
};

static void displayHelpText(const juce::ArgumentList& args)
{
    std::clog << "Usage:\n"
                 "    " << args.executableName << " [--output|-o] <output.xml> --width=<width> --layer_n=<layer_n> [--reverse]\n"
              << std::endl;
}

int main(int argc, char* argv[])
{
    // Parse args
    juce::ArgumentList args(argc, argv);
    if(args.size() < 3)
    {
        displayHelpText(args);
        exit(EXIT_FAILURE);
    }
    if(args.containsOption("--help|-h"))
    {
        displayHelpText(args);
        exit(EXIT_SUCCESS);
    }

    bool reverse = args.removeOptionIfFound("--reverse");
    int width = -1;
    int layer_n = -1;
    {
        const juce::String width_str = args.removeValueForOption("--width");
        const juce::String layer_n_str = args.removeValueForOption("--layer_n");
        width = width_str.getIntValue();
        layer_n = layer_n_str.getIntValue();
        if(width_str.isEmpty() || layer_n_str.isEmpty()
            || width <= 0 || layer_n < 0)
        {
            std::clog << "Error: width and layer numbers must be provided and greater than 0" << std::endl;
            displayHelpText(args);
            exit(EXIT_FAILURE);
        }
    }

    juce::String output_path;
    if(args.containsOption("--output|-o"))
    {
        output_path = args.getValueForOption("--output|-o");
    }
    else
    {
        output_path = args[0].text;
    }

    if(output_path.isEmpty())
    {
        displayHelpText(args);
        exit(EXIT_FAILURE);
    }

    juce::File output_file(output_path);
    if(output_file.isDirectory())
    {
        std::clog << "Error: output path `" << output_path << "` is a directory." << std::endl;
        displayHelpText(args);
        exit(EXIT_FAILURE);
    }
    if(output_file.exists())
    {
        std::clog << "Warning: output file already exists and will be overwritten: " << output_file.getFullPathName() << std::endl;
        if(!output_file.deleteFile())
        {
            std::clog << "Error: File couldn't be cleared." << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    if(!output_file.create())
    {
        std::clog << "Error: failed to create output file: " << output_file.getFullPathName() << std::endl;
        exit(EXIT_FAILURE);
    }

    // Stuff required for initializing the state manager.
    StubAudioProcessor _;
    juce::AudioProcessorValueTreeState parameters(
        _, nullptr, Name::ROOT, SynthStateManager::createParameterLayout()
    );
    SynthStateManager::Meta state_manager_meta;
    SynthStateManager state_manager(parameters, state_manager_meta);
    state_manager.regenerateElementTrees();


    // --- Lattice generation ---
    // Generate elements

    // This holds the IDs of the created elements to aid in later connecting them in a pattern.
    // It would be possible to accomplish the same task with some math and modulo, but this is just easier to implement,
    // especially considering the reverse option and signedness of the IDs.
    // + 1 for oscillators
    ElementID id_layers[layer_n + 1][width];
    // First 0-initialize to help catch errors
    for(int i = 0; i < layer_n + 1; i++)
        for(int j = 0; j < width; j++)
            id_layers[i][j] = 0;

    // Oscillators and sinks
    for(int i = 0; i < width; i++)
    {
        id_layers[0][i] = state_manager.addElementRep(ElementCategory::GENERATOR, ElementType::GEN_OSCILLATOR);
        state_manager.addElementRep(ElementCategory::SINK, ElementType::SINK_BITMIX);
    }
    // Gates
    constexpr auto gate_type = ElementType::GATE_XOR;
    if(!reverse)
    {
        for(int i = 1; i < layer_n + 1; i++)
            for(int j = 0; j < width; j++)
                id_layers[i][j] = state_manager.addElementRep(ElementCategory::PROCESSOR, gate_type);
    }
    else
    {
        for(int i = layer_n; i >= 1; i--)
            for(int j = width - 1; j >= 0; j--)
                id_layers[i][j] = state_manager.addElementRep(ElementCategory::PROCESSOR, gate_type);
    }

    // Generate connections for gates
    for(int i = 1; i < layer_n + 1; i++)
    {
        for(int j = 0; j < width; j++)
        {
            const ElementID target_element_id = id_layers[i][j];
            const ElementID source_element_id_0 = id_layers[i - 1][j];
            const ElementID source_element_id_1 = id_layers[i - 1][(j + 1) % width];
            state_manager.setConnection(
                createConnectionID(source_element_id_0, 0),
                createConnectionID(target_element_id, 0)
            );
            state_manager.setConnection(
                createConnectionID(source_element_id_1, 0),
                createConnectionID(target_element_id, 1)
            );
        }
    }
    // Connections for sinks
    for(int j = 0; j < width; j++)
    {
        const ElementID source_element_id = id_layers[layer_n][j];
        const ElementID target_element_id = applySignElement(j + 1, SIGN_SINK);
        state_manager.setConnection(
            createConnectionID(source_element_id, 0),
            createConnectionID(target_element_id, 0)
        );
    }

    // TODO - maybe add some oscillator parameter variation?

    // Save to file
    {
        juce::XmlElement::TextFormat format;
        format.newLineChars = "\n";
        auto xml_element = parameters.state.createXml();
        if(xml_element == nullptr)
        {
            std::clog << "Error: failed to create XML element from state." << std::endl;
            exit(EXIT_FAILURE);
        }
        if(!xml_element->writeTo(output_file, format))
        {
            std::clog << "Error: failed to write output file: " << output_file.getFullPathName() << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    exit(EXIT_SUCCESS);
}

