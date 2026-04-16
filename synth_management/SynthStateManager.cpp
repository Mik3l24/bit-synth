//
// Created by micha on 15.04.2026.
//

#include "SynthStateManager.h"

constexpr size_t PLACEHOLDER_PARAMETER_N = 1024;


juce::ValueTree SynthStateManager::newOscillatorRep(ElementID id)
{
    return juce::ValueTree(name::OSCILLATOR,
        {
            {name::ID,             id},
            {name::INDEX,          -id-1},
            {name::RATIO,
                registerDynamicParameter(juce::String::formatted("Oscillator %d Ratio", id))},
            {name::STARTING_PHASE,
                registerDynamicParameter(juce::String::formatted("Oscillator %d Starting Phase", id))},
            {name::PULSE_WIDTH,
                registerDynamicParameter(juce::String::formatted("Oscillator %d Pulse Width", id))}
        }
    );
}

inline juce::ValueTree newConnectionRep(ConnectionID id)
{
    return juce::ValueTree(name::CONNECTION,
       {
           {name::ID, id},
       }
    );
}

juce::ValueTree SynthStateManager::newGateRep(ElementID id, const GateType type)
{
    const auto input_n = gateMaxInputN(type);
    juce::ValueTree connections_tree(name::CONNECTIONS);
    for(size_t i = 0; i < input_n; i++)
        connections_tree.appendChild(newConnectionRep(CONNECTION_NONE), nullptr);

    const juce::Identifier type_name = toGateIdentifier(type);
    return juce::ValueTree(type_name,
        {
            {name::ID, id},
            {name::INDEX, id-1},
            {name::INPUT_N, input_n},
        },
        {
            std::move(connections_tree)
        }
     );
}

juce::ValueTree SynthStateManager::newBitMixChannelRep(ElementID id)
{
    return juce::ValueTree(name::MIX_CHANNEL,
        {
            {name::ID, id},
            {name::INDEX, id-1},
            {name::CONNECTIONS, CONNECTION_NONE},
            {name::LEVEL,
            registerDynamicParameter(juce::String::formatted("BitMix Channel %d Level", id))},
        }
    );
}

juce::String SynthStateManager::registerDynamicParameter(juce::String friendly_name)
{
    if(state.next_free_dynamic_parameter_id >= PLACEHOLDER_PARAMETER_N)
        throw std::runtime_error("No more dynamic parameters can be registered");
    const auto key = juce::String::formatted("#%04x", state.next_free_dynamic_parameter_id);
    auto dyn_param = parameters.getParameter(key);
    if(dyn_param == nullptr)
        throw std::runtime_error(("Parameter '"+key+"' does not exist").toStdString());

    // dyn_param->name = friendly_name;
    //Ok, so it turns out that the built-in JUCE parameters' name cannot be changed after creation.
    // I guess I'm gonna have to TODO custom AudioProcessorParameter class that allows mutation
    // (though, this one is gonna work for now, it's just going to be obtuse for the user to set up automation)

    state.next_free_dynamic_parameter_id++;
    return key;
}


ElementID SynthStateManager::addElementRep(const ElementType element_type, const GateType gate_type)
{
    const auto& element_container_identifier = toContainerIdentifier(element_type);
    jassert(element_container_identifier != juce::Identifier());
    juce::ValueTree element_container = parameters.state.getChildWithName(element_container_identifier);
    jassert(element_container.isValid());

    const auto num = element_container.getNumChildren();
    const ElementID new_id = num + 1;
    
    juce::ValueTree new_tree;
    switch(element_type)
    {
        case ElementType::GENERATOR:
            new_tree = newOscillatorRep(new_id);
            break;
        case ElementType::COMPONENT:
            new_tree = newGateRep(new_id, gate_type);
            break;
        case ElementType::SINK:
            new_tree = newBitMixChannelRep(new_id);
            break;
        default:
            jassertfalse;
    }

    element_container.appendChild(new_tree, nullptr);
    return new_id;
}

void SynthStateManager::setConnection(ConnectionID source_id, ConnectionID target_id)
{
    jassertfalse; // Not implemented yet
}

juce::AudioProcessorValueTreeState::ParameterLayout SynthStateManager::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout params =
    {
        // Static parameters of the synth
        std::make_unique<juce::AudioParameterFloat>(
            name::MASTER_LEVEL.getCharPointer(), "Master volume",
            0.0f, 1.0f, 0.5f
            ),
    };
    for(size_t i = 0; i < PLACEHOLDER_PARAMETER_N; i++)
    {
        const auto key = juce::String::formatted("#%04x",i);
        const auto friendly_name = "Unassigned " + key;
        params.add(std::make_unique<juce::AudioParameterFloat>(key, friendly_name, 0.0f, 1.0f, 0.0f));
        std::cout << key << std::endl;
    }
    return params;
}
