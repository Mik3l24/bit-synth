#include "DragSourceType.h"
#include "Theme.h"
#include "synth_management/Errors.h"
//[/Headers]

#include "StructureEditor.h"

namespace ui {

// Creates a path that only follows cardinal directions
static juce::Path taxiwayPath(const juce::Point<int>& start, const juce::Point<int>& end)
{
    const int min_distance = 20; // TODO Move to the theme class?

    juce::Path path;
    path.startNewSubPath(start.toFloat());

    const bool to_right = start.getX() - end.getX() > 2 * min_distance;

    const int midpoint_x = to_right
        ? (start.getX() + end.getX()) / 2
        : start.getX() - min_distance;
    const int midpoint_y = to_right
        ? end.getY()
        : (start.getY() + end.getY()) / 2; // could make this more skewed to the end for small distances

    path.lineTo(float(midpoint_x), float(start.getY()));
    path.lineTo(float(midpoint_x), float(midpoint_y));
    if(!to_right)
    {
        path.lineTo(float(end.getX() + min_distance), float(midpoint_y));
        path.lineTo(float(end.getX() + min_distance), float(end.getY()));
    }
    path.lineTo(end.toFloat());
    return path;
}

StructureEditor::StructureEditor(const SynthStateManager& state_manager)
    : state_manager(state_manager)
{
    picker.reset (new ElementPicker());
    addAndMakeVisible (picker.get());

    setSize (600, 400);
}

StructureEditor::~StructureEditor()
{
    picker = nullptr;
}

void StructureEditor::paint (juce::Graphics& g)
{
    g.fillAll (Theme::getStructureBackground());

    // Paint lines between sources and targets
    g.setColour(Theme::getStructureLogicForeground());
    const float stroke_thickness = Theme::getStructureConnectionStrokeThickness();
    for(auto& gate : processor_components)
    {
        for(auto i = 0; i <= 1; i++)
        {
            std::optional<std::pair<juce::Point<int>, juce::Point<int>>> points = gate->getConnectionPoints(i);
            if(points.has_value())
            {
                g.strokePath(taxiwayPath(
                    getLocalPoint(nullptr,points.value().first),
                    getLocalPoint(nullptr,points.value().second)),
                           juce::PathStrokeType(stroke_thickness));
            }
        }
    }
    for(auto& channel : mix_components)
    {
        std::optional<std::pair<juce::Point<int>, juce::Point<int>>> points = channel->getConnectionPoints();
        if(points.has_value())
        {
            g.strokePath(taxiwayPath(
                    getLocalPoint(nullptr,points.value().first),
                    getLocalPoint(nullptr,points.value().second)),
                           juce::PathStrokeType(stroke_thickness));
        }
    }
}

void StructureEditor::resized()
{
    picker->setCentrePosition(getWidth() / 2, picker->getHeight() / 2);
}

void StructureEditor::rebuildUI()
{
    osc_components.clear();
    processor_components.clear();
    mix_components.clear();

    jassert(state_manager.parameters.state.isValid());
    const juce::ValueTree generators = state_manager.parameters.state.getChildWithName(Name::GENERATORS);
    const juce::ValueTree processors = state_manager.parameters.state.getChildWithName(Name::PROCESSORS);
    const juce::ValueTree sinks      = state_manager.parameters.state.getChildWithName(Name::SINKS);
    if(!generators.isValid() || !processors.isValid() || !sinks.isValid())
    {
        // Then we are almost definitely starting from a clean patch,
        // where this callback is called before these trees are recreated.
        // Just to be safe, the elements will still be cleared in case the editor wasn't also recreated.
        // NOTE - this will lead to problems if the patch was in the invalid state of having only some of these trees.
        // Though, that should be detected in the code for loading the patch
        return;
    }

    // Let's start recreating!
    for(const auto& generator : generators)
    {
        jassert(generator.isValid());
        if(generator.getType() == Name::OSCILLATOR)
        {
            throwassert(generator.hasProperty(Name::ID),
                        InvalidTreeError("No valid ID for generator in generators tree"));
            const ElementID id = generator[Name::ID];
            throwassert(matchesSign(id, SIGN_GENERATOR),
                        InvalidTreeError("Invalid ID for generator in generators tree"));

            const juce::Point<int> position =
                generator.hasProperty(Name::META_UI_POSITION_X) && generator.hasProperty(Name::META_UI_POSITION_Y)
                ? juce::Point<int>(generator[Name::META_UI_POSITION_X], generator[Name::META_UI_POSITION_Y])
                : juce::Point<int>(getWidth() / 2, getHeight() / 2); // Default position if not specified

            addElementComponent(id, position, ElementCategory::GENERATOR, GateType::NONE, false);
        }
        else throw InvalidTreeError("Invalid child type in generators tree: "+generator.getType());
    }
    for(const auto& processor : processors)
    {
        jassert(processor.isValid());
        if(isIdentifierAGate(processor.getType()))
        {
            throwassert(processor.hasProperty(Name::ID),
                        InvalidTreeError("No valid ID for component in components tree"));
            const ElementID id = processor[Name::ID];
            throwassert(matchesSign(id, SIGN_PROCESSOR),
                        InvalidTreeError("Invalid ID for component in components tree"));

            const juce::Point<int> position =
                processor.hasProperty(Name::META_UI_POSITION_X) && processor.hasProperty(Name::META_UI_POSITION_Y)
                ? juce::Point<int>(processor[Name::META_UI_POSITION_X], processor[Name::META_UI_POSITION_Y])
                : juce::Point<int>(getWidth() / 2, getHeight() / 2); // Default position if not specified

            addElementComponent(id, position, ElementCategory::PROCESSOR, toGateEnum(processor.getType()), false);
        }
        else throw InvalidTreeError("Invalid child type in components tree: "+processor.getType());
    }
    for(const auto& sink : sinks)
    {
        jassert(sink.isValid());
        if(sink.getType() == Name::MIX_CHANNEL)
        {
            throwassert(sink.hasProperty(Name::ID),
                        InvalidTreeError("No valid ID for sink in sinks tree"));
            const ElementID id = sink[Name::ID];
            throwassert(matchesSign(id, SIGN_SINK),
                        InvalidTreeError("Invalid ID for sink in sinks tree"));

            const juce::Point<int> position =
                sink.hasProperty(Name::META_UI_POSITION_X) && sink.hasProperty(Name::META_UI_POSITION_Y)
                ? juce::Point<int>(sink[Name::META_UI_POSITION_X], sink[Name::META_UI_POSITION_Y])
                : juce::Point<int>(getWidth() / 2, getHeight() / 2); // Default position if not specified

            addElementComponent(id, position, ElementCategory::SINK, GateType::NONE, false);
        }
        else throw InvalidTreeError("Invalid child type in sinks tree: "+sink.getType());
    }

    // Remaking connections
    for(const auto& processor : processors)
    {
        jassert(processor.isValid());
        if(isIdentifierAGate(processor.getType()))
        {
            const ElementID id = processor[Name::ID];
            for(const auto& connection : processor.getChildWithName(Name::CONNECTIONS))
            {
                jassert(connection.isValid());
                throwassert(connection.hasProperty(Name::ID),
                            InvalidTreeError("Invalid connection ID in component connections"));
                const ConnectionID source_id = connection[Name::ID];
                const SubConnectionID target_sub_id = processor.getChildWithName(Name::CONNECTIONS).indexOf(connection); // Kinda dumb

                const auto* const processor_ptr = findProcessorByID(id);
                jassert(processor_ptr != nullptr);
                jassert(target_sub_id < gateMaxInputN(processor_ptr->type));
                auto* const target_connector = target_sub_id == 0 ? processor_ptr->target0.get() : processor_ptr->target1.get(); // Will need a refactor for different numbers of inputs
                jassert(target_connector != nullptr);

                if(auto* source = findSourceByID(source_id))
                    target_connector->makeConnection(source, false);

            }
        }
        else throw InvalidTreeError("Invalid child type in components tree: "+processor.getType());
    }
    for(const auto& sink : sinks)
    {
        jassert(sink.isValid());
        if(sink.getType() == Name::MIX_CHANNEL)
        {
            const ElementID id = sink[Name::ID];
            for(const auto& connection : sink.getChildWithName(Name::CONNECTIONS))
            {
                jassert(connection.isValid());
                throwassert(connection.hasProperty(Name::ID),
                            InvalidTreeError("Invalid connection ID in sink connections"));
                const ConnectionID source_id = connection[Name::ID];

                const auto* const sink_ptr = findSinkByID(id);
                jassert(sink_ptr != nullptr);
                auto* const target_connector = sink_ptr->target.get();
                jassert(target_connector != nullptr);

                if(auto* source = findSourceByID(source_id))
                    target_connector->makeConnection(source, false);
            }
        }
        else throw InvalidTreeError("Invalid child type in sinks tree: "+sink.getType());
    }
}


bool StructureEditor::isInterestedInDragSource(const juce::DragAndDropTarget::SourceDetails& dragSourceDetails)
{
    return dragSourceDetails.description.equals(DragSourceType::ELEMENT_ADDER_BUTTON);
}

void StructureEditor::itemDropped(const juce::DragAndDropTarget::SourceDetails& dragSourceDetails)
{
    auto* button = dynamic_cast<ElementAdderButton*>(dragSourceDetails.sourceComponent.get());
    if(button == nullptr)
    {
        std::cout << "StructureEditor::itemDropped: source is not ElementAdderButton" << std::endl;
        return;
    }

    juce::Component* component = nullptr;
    const ElementCategory element = button->getElementType();

    //dragSourceDetails.localPosition

    addNewElement(dragSourceDetails.localPosition.toInt(), element, button->getGateType());

}

void StructureEditor::addElementComponent(ElementID id, juce::Point<int> position, ElementCategory element_type, GateType gate_type, const bool position_is_center)
{
    juce::Component* component = nullptr;
    switch(element_type)
    {
        case ElementCategory::GENERATOR:
        {
            osc_components.emplace_back(new OscillatorParameters(id, state_manager));
            component = osc_components.back().get();
            break;
        }
        case ElementCategory::PROCESSOR:
        {
            processor_components.emplace_back(new Gate(id, gate_type, state_manager));
            component = processor_components.back().get();
            break;
        }
        case ElementCategory::SINK:
        {
            mix_components.emplace_back(new MixChannelParameters(id, state_manager));
            component = mix_components.back().get();
            break;
        }
    }
    jassert(component != nullptr);

    // Add the component and place it at the mouse position
    addAndMakeVisible(component);
    if(position_is_center)
        component->setCentrePosition(position);
    else
        component->setTopLeftPosition(position);
}

OscillatorParameters* StructureEditor::findGeneratorByID(const ElementID id) const
{
    jassert(matchesSign(id, SIGN_GENERATOR));
    // Most likely, the ID corresponds to its index.
    const auto index = getElementIndex(id);
    if(auto* generator = osc_components[index].get(); generator != nullptr && generator->id == id)
        return generator;

    // Then we have to do the slow search...
    for(const auto& generator : osc_components)
        if(generator->id == id)
            return generator.get();
    return nullptr;
}

Gate* StructureEditor::findProcessorByID(const ElementID id) const
{
    jassert(matchesSign(id, SIGN_PROCESSOR));
    // Most likely, the ID corresponds to its index.
    const auto index = getElementIndex(id);
    if(auto* processor = processor_components[index].get(); processor != nullptr && processor->id == id)
        return processor;

    // Then we have to do the slow search...
    for(const auto& processor : processor_components)
        if(processor->id == id)
            return processor.get();
    return nullptr;
}

MixChannelParameters* StructureEditor::findSinkByID(ElementID id) const
{
    jassert(matchesSign(id, SIGN_SINK));
    // Most likely, the ID corresponds to its index.
    const auto index = getElementIndex(id);
    if(auto* sink = mix_components[index].get(); sink != nullptr && sink->id == id)
        return sink;

    // Then we have to do the slow search...
    for(const auto& sink : mix_components)
        if(sink->id == id)
            return sink.get();
    return nullptr;
}

SourceConnector* StructureEditor::findSourceByID(const ConnectionID id) const
{
    auto [element_id, sub_id, sign] = decodeConnectionID(id);
    jassert(sub_id == 0); // if we ever have multi-output elements, this will need to be updated
    if(matchesSign(id, SIGN_GENERATOR))
        if(const auto* generator = findGeneratorByID(element_id); generator != nullptr)
            return generator->source_connector.get();
    if(matchesSign(id, SIGN_PROCESSOR))
        if(const auto* processor = findProcessorByID(element_id); processor != nullptr)
            return processor->source.get();
    return nullptr;
}

}

