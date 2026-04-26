/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 7.0.5

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
#include "DragSourceType.h"
#include "Theme.h"
#include "synth_management/Errors.h"
//[/Headers]

#include "StructureEditor.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
namespace ui {

// Creates a path that only follows cardinal directions
juce::Path taxiwayPath(const juce::Point<int>& start, const juce::Point<int>& end)
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

//[/MiscUserDefs]

//==============================================================================
StructureEditor::StructureEditor(const SynthStateManager& state_manager)
    : state_manager(state_manager)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    picker.reset (new ElementPicker());
    addAndMakeVisible (picker.get());

    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..

    //[/Constructor]
}

StructureEditor::~StructureEditor()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    picker = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void StructureEditor::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Theme::getStructureBackground());

    //[UserPaint] Add your own custom painting code here..
    // Paint lines between sources and targets
    g.setColour(Theme::getStructureLogicForeground());
    const float stroke_thickness = Theme::getStructureConnectionStrokeThickness();
    for(auto& gate : gate_components)
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
    //[/UserPaint]
}

void StructureEditor::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    picker->setCentrePosition(getWidth() / 2, picker->getHeight() / 2);

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void StructureEditor::rebuildFromTree()
{
    osc_components.clear();
    gate_components.clear();
    mix_components.clear();

    jassert(state_manager.parameters.state.isValid());
    const juce::ValueTree generators = state_manager.parameters.state.getChildWithName(name::GENERATORS);
    const juce::ValueTree components = state_manager.parameters.state.getChildWithName(name::COMPONENTS);
    const juce::ValueTree sinks      = state_manager.parameters.state.getChildWithName(name::SINKS);
    if(!generators.isValid() || !components.isValid() || !sinks.isValid())
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
        if(generator.getType() == name::OSCILLATOR)
        {
            throwassert(generator.hasProperty(name::ID),
                        InvalidTreeError("No valid ID for generator in generators tree"));
            const ElementID id = generator[name::ID];
            throwassert(matchesSign(id, SIGN_GENERATOR),
                        InvalidTreeError("Invalid ID for generator in generators tree"));

            const juce::Point<int> position =
                generator.hasProperty(name::META_UI_POSITION_X) && generator.hasProperty(name::META_UI_POSITION_Y)
                ? juce::Point<int>(generator[name::META_UI_POSITION_X], generator[name::META_UI_POSITION_Y])
                : juce::Point<int>(getWidth() / 2, getHeight() / 2); // Default position if not specified

            addElementComponent(id, position, ElementType::GENERATOR);
        }
        else throw InvalidTreeError("Invalid child type in generators tree: "+generator.getType());
    }
    for(const auto& component : components)
    {
        jassert(component.isValid());
        if(isIdentifierAGate(component.getType()))
        {
            throwassert(component.hasProperty(name::ID),
                        InvalidTreeError("No valid ID for component in components tree"));
            const ElementID id = component[name::ID];
            throwassert(matchesSign(id, SIGN_COMPONENT),
                        InvalidTreeError("Invalid ID for component in components tree"));

            const juce::Point<int> position =
                component.hasProperty(name::META_UI_POSITION_X) && component.hasProperty(name::META_UI_POSITION_Y)
                ? juce::Point<int>(component[name::META_UI_POSITION_X], component[name::META_UI_POSITION_Y])
                : juce::Point<int>(getWidth() / 2, getHeight() / 2); // Default position if not specified

            addElementComponent(id, position, ElementType::COMPONENT, toGateEnum(component.getType()));
        }
        else throw InvalidTreeError("Invalid child type in components tree: "+component.getType());
    }
    for(const auto& sink : sinks)
    {
        jassert(sink.isValid());
        if(sink.getType() == name::MIX_CHANNEL)
        {
            throwassert(sink.hasProperty(name::ID),
                        InvalidTreeError("No valid ID for sink in sinks tree"));
            const ElementID id = sink[name::ID];
            throwassert(matchesSign(id, SIGN_SINK),
                        InvalidTreeError("Invalid ID for sink in sinks tree"));

            const juce::Point<int> position =
                sink.hasProperty(name::META_UI_POSITION_X) && sink.hasProperty(name::META_UI_POSITION_Y)
                ? juce::Point<int>(sink[name::META_UI_POSITION_X], sink[name::META_UI_POSITION_Y])
                : juce::Point<int>(getWidth() / 2, getHeight() / 2); // Default position if not specified

            addElementComponent(id, position, ElementType::SINK);
        }
        else throw InvalidTreeError("Invalid child type in sinks tree: "+sink.getType());
    }

    // Remaking connections
    for(const auto& component : components)
    {
        jassert(component.isValid());
        if(isIdentifierAGate(component.getType()))
        {
            const ElementID id = component[name::ID];
            for(const auto& connection : component.getChildWithName(name::CONNECTIONS))
            {
                jassert(connection.isValid());
                throwassert(connection.hasProperty(name::ID),
                            InvalidTreeError("Invalid connection ID in component connections"));
                const ConnectionID source_id = connection[name::ID];
                const SubConnectionID target_sub_id = component.getChildWithName(name::CONNECTIONS).indexOf(connection); // Kinda dumb

                const auto* const component_ptr = findComponentByID(id);
                jassert(component_ptr != nullptr);
                jassert(target_sub_id < gateMaxInputN(component_ptr->type));
                auto* const target_connector = target_sub_id == 0 ? component_ptr->target0.get() : component_ptr->target1.get(); // Will need a refactor for different numbers of inputs
                jassert(target_connector != nullptr);

                if(auto* source = findSourceByID(source_id))
                    target_connector->makeConnection(source, false);

            }
        }
        else throw InvalidTreeError("Invalid child type in components tree: "+component.getType());
    }
    for(const auto& sink : sinks)
    {
        jassert(sink.isValid());
        if(sink.getType() == name::MIX_CHANNEL)
        {
            const ElementID id = sink[name::ID];
            for(const auto& connection : sink.getChildWithName(name::CONNECTIONS))
            {
                jassert(connection.isValid());
                throwassert(connection.hasProperty(name::ID),
                            InvalidTreeError("Invalid connection ID in sink connections"));
                const ConnectionID source_id = connection[name::ID];

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


//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
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
    const ElementType element = button->getElementType();

    //dragSourceDetails.localPosition

    addNewElement(dragSourceDetails.localPosition.toInt(), element, button->getGateType());

}

void StructureEditor::addElementComponent(ElementID id, juce::Point<int> position, ElementType element_type, GateType gate_type)
{
    juce::Component* component = nullptr;
    switch(element_type)
    {
        case ElementType::GENERATOR:
        {
            osc_components.emplace_back(new OscillatorParameters(id, state_manager));
            component = osc_components.back().get();
            break;
        }
        case ElementType::COMPONENT:
        {
            gate_components.emplace_back(new Gate(id, gate_type, state_manager));
            component = gate_components.back().get();
            break;
        }
        case ElementType::SINK:
        {
            mix_components.emplace_back(new MixChannelParameters(id, state_manager));
            component = mix_components.back().get();
            break;
        }
    }
    jassert(component != nullptr);

    // Add the component and place it at the mouse position
    addAndMakeVisible(component);
    component->setCentrePosition(position);
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

Gate* StructureEditor::findComponentByID(const ElementID id) const
{
    jassert(matchesSign(id, SIGN_COMPONENT));
    // Most likely, the ID corresponds to its index.
    const auto index = getElementIndex(id);
    if(auto* component = gate_components[index].get(); component != nullptr && component->id == id)
        return component;

    // Then we have to do the slow search...
    for(const auto& component : gate_components)
        if(component->id == id)
            return component.get();
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
    if(matchesSign(id, SIGN_COMPONENT))
        if(const auto* component = findComponentByID(element_id); component != nullptr)
            return component->source.get();
    return nullptr;
}

//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="StructureEditor" componentName=""
                 parentClasses="public juce::Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff505050"/>
  <JUCERCOMP name="" id="bd33bf19f618d685" memberName="picker" virtualName=""
             explicitFocusOrder="0" pos="0Cc 0 60% 24" sourceFile="ElementPicker.cpp"
             constructorParams=""/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
}
//[/EndFile]

