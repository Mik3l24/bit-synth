//
// Created by micha on 15.04.2026.
//

#include "SynthStateManager.h"

#include "Errors.h"

constexpr size_t PLACEHOLDER_PARAMETER_N = 1024;

juce::String SynthStateManager::registerDynamicParameter(juce::String friendly_name) const
{
    const size_t next_id = getNextDynamicParameterID();
    throwassert(next_id < PLACEHOLDER_PARAMETER_N,
                std::range_error("No more dynamic parameters can be registered"));
    const auto key = juce::String::formatted("#%04x", next_id);
    auto dyn_param = parameters.getParameter(key);
    throwassert(dyn_param != nullptr,
                InvalidTreeError(("Parameter '"+key+"' does not exist")));

    // dyn_param->name = friendly_name;
    //Ok, so it turns out that the built-in JUCE parameters' name cannot be changed after creation.
    // I guess I'm gonna have to TODO custom AudioProcessorParameter class that allows mutation
    // (though, this one is gonna work for now, it's just going to be obtuse for the user to set up automation)

    setNextFreeDynamicParameterID(next_id + 1);
    return key;
}

void SynthStateManager::setConnection(const ConnectionID source_id, const ConnectionID target_id) const
{
    auto [target_element_id, target_subconnection_id, target_sign] = decodeConnectionID(target_id);
    juce::ValueTree& target_tree = matchesSign(target_element_id, SIGN_PROCESSOR) ? meta.processors : meta.sinks;
    jassert(target_tree.isValid());
    const auto& final =
                target_tree.getChildWithProperty(Name::ID, target_element_id)
                .getChildWithName(Name::CONNECTIONS).getChild(target_subconnection_id)
                .setProperty(Name::ID, source_id, nullptr);
    jassert(final.isValid());
}

void SynthStateManager::setElementPosition(ElementID id, ElementCategory element_type, juce::Point<int> position) const
{
    juce::ValueTree element = getElementContainer(element_type).getChildWithProperty(Name::ID, id);
    jassert(element.isValid());
    element.setProperty(Name::META_UI_POSITION_X, position.x, nullptr);
    element.setProperty(Name::META_UI_POSITION_Y, position.y, nullptr);
}

juce::AudioProcessorValueTreeState::ParameterLayout SynthStateManager::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout params =
    {
        // Static parameters of the synth
        std::make_unique<juce::AudioParameterFloat>(
            Name::MASTER_LEVEL.getCharPointer(), "Master volume",
            0.0f, 1.0f, 0.5f
            ),
    };
    for(size_t i = 0; i < PLACEHOLDER_PARAMETER_N; i++)
    {
        const auto key = juce::String::formatted("#%04x",i);
        const auto friendly_name = "Unassigned " + key;
        params.add(std::make_unique<juce::AudioParameterFloat>(key, friendly_name, 0.0f, 1.0f, 0.0f));
    }
    return params;
}

size_t SynthStateManager::getNextDynamicParameterID() const
{
    return size_t(juce::int64(parameters.state.getChildWithName(Name::META_STATE)[Name::META_NEXT_FREE_DYNAMIC_PARAMETER_ID]));
}

void SynthStateManager::setNextFreeDynamicParameterID(const size_t id) const
{
    parameters.state.getChildWithName(Name::META_STATE).setProperty(Name::META_NEXT_FREE_DYNAMIC_PARAMETER_ID, juce::int64(id), nullptr);
}

juce::ValueTree SynthStateManager::newOscillatorRep(ElementID id) const
{
    return juce::ValueTree(Name::OSCILLATOR,
        {
            {Name::ID,             id},
            {Name::INDEX,          -id-1},
            {Name::RATIO,
                registerDynamicParameter(juce::String::formatted("Oscillator %d Ratio", id))},
            {Name::STARTING_PHASE,
                registerDynamicParameter(juce::String::formatted("Oscillator %d Starting Phase", id))},
            {Name::PULSE_WIDTH,
                registerDynamicParameter(juce::String::formatted("Oscillator %d Pulse Width", id))}
        }
    );
}

inline juce::ValueTree newConnectionRep(ConnectionID id)
{
    return juce::ValueTree(Name::CONNECTION,
       {
           {Name::ID, id},
       }
    );
}

juce::ValueTree SynthStateManager::newGateRep(ElementID id, const GateType type) const
{
    const auto input_n = gateMaxInputN(type);
    juce::ValueTree connections_tree(Name::CONNECTIONS);
    for(size_t i = 0; i < input_n; i++)
        connections_tree.appendChild(newConnectionRep(CONNECTION_NONE), nullptr);

    const juce::Identifier type_name = toGateIdentifier(type);
    return juce::ValueTree(type_name,
        {
            {Name::ID, id},
            {Name::INDEX, id-1},
            {Name::INPUT_N, input_n},
        },
        {
            std::move(connections_tree)
        }
     );
}

juce::ValueTree SynthStateManager::newBitMixChannelRep(const ElementID id) const
{
    return juce::ValueTree(Name::MIX_CHANNEL,
        {
            {Name::ID, id},
            {Name::INDEX, -id-1},
            {Name::LEVEL,
            registerDynamicParameter(juce::String::formatted("BitMix Channel %d Level", id))},
        },
        {
            {
                Name::CONNECTIONS, {},
                {
                    {Name::CONNECTION, {{Name::ID, CONNECTION_NONE}}}
                }
            }
        }

    );
}

ElementID SynthStateManager::addElementRep(const ElementCategory element_type, const GateType gate_type) const
{
    juce::ValueTree& element_container = getElementContainer(element_type);
    jassert(element_container.isValid());

    const auto num = element_container.getNumChildren();
    const ElementID new_id = createElementID(num, element_type);

    juce::ValueTree new_tree;
    switch(element_type)
    {
        case ElementCategory::GENERATOR:
            new_tree = newOscillatorRep(new_id);
            break;
        case ElementCategory::PROCESSOR:
            new_tree = newGateRep(new_id, gate_type);
            break;
        case ElementCategory::SINK:
            new_tree = newBitMixChannelRep(new_id);
            break;
        default:
            jassertfalse;
    }

    element_container.appendChild(new_tree, nullptr);
    return new_id;
}

juce::ValueTree& SynthStateManager::getElementContainer(const ElementCategory element_type) const
{
    switch(element_type)
    {
        case ElementCategory::GENERATOR:
            return meta.generators;
        case ElementCategory::PROCESSOR:
            return meta.processors;
        case ElementCategory::SINK:
            return meta.sinks;
        default:
            jassertfalse;
            return meta.generators; // To silence compiler warning
    }
}

