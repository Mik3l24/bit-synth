//
// Created by micha on 15.04.2026.
//

#include "SynthStateManager.h"

#include "Errors.h"

constexpr size_t DYNAMIC_PARAMETER_SET_N = 32;

struct ParamInfo
{
    const juce::Identifier& name;
    float min, max, default_value;
};

const std::map<juce::Identifier, std::vector<ParamInfo>> param_map =
{
    {Name::OSCILLATOR,
        {
            {Name::RATIO, 0.0001f, 40, 1},
            {Name::PULSE_WIDTH, 0, 1, .5f},
            {Name::STARTING_PHASE, 0, juce::MathConstants<float>::twoPi, 0}
        }
    },
    {Name::MIX_CHANNEL,
        {
            {Name::LEVEL, 0, 1, .75f}
        }
    },
};

[[nodiscard]] static inline juce::String getDynamicParameterNameID(const juce::Identifier& type, const juce::Identifier& param_name, ElementID element_id);
[[nodiscard]] static inline juce::String getDynamicParameterNameStr(const juce::Identifier& type, const juce::Identifier& param_name, juce::StringRef element_id);
[[nodiscard]] static inline juce::String formatID(ElementID id);


juce::String SynthStateManager::registerDynamicParameter(juce::String friendly_name)
{
    const size_t next_id = getNextDynamicParameterID();
    throwassert(next_id < DYNAMIC_PARAMETER_SET_N,
                std::range_error("No more dynamic parameters can be registered"));
    const auto key = juce::String::formatted("#%04x", next_id);
    auto dyn_param = parameters.getParameter(key);
    throwassert(dyn_param != nullptr,
                InvalidTreeError(("Parameter '"+key+"' does not exist")));

    // dyn_param->name = friendly_name;
    //Ok, so it turns out that the built-in JUCE parameters' name cannot be changed after creation.
    // I guess I'm gonna have to T/O/D/O custom AudioProcessorParameter class that allows mutation
    // (though, this one is gonna work for now, it's just going to be obtuse for the user to set up automation)
    // No longer a concern in the new approach!
    // Though, a modified version of this function may still be useful when more sinks/generators are implemented (see hack)

    setNextFreeDynamicParameterID(next_id + 1);
    return key;
}

void SynthStateManager::setConnection(const ConnectionID source_id, const ConnectionID target_id)
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

void SynthStateManager::setElementPosition(const ElementID id, const ElementCategory element_category, const juce::Point<int> position)
{
    juce::ValueTree element = getElementContainer(element_category).getChildWithProperty(Name::ID, id);
    jassert(element.isValid());
    element.setProperty(Name::META_UI_POSITION_X, position.x, nullptr);
    element.setProperty(Name::META_UI_POSITION_Y, position.y, nullptr);
}

juce::AudioProcessorValueTreeState::ParameterLayout SynthStateManager::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout =
    {
        // Static parameters of the synth
        std::make_unique<juce::AudioParameterFloat>(
            Name::MASTER_LEVEL.getCharPointer(), "Master volume",
            0.0f, 1.0f, 0.5f
            ),
    };
    // Dynamic parameters.
    // Current approach - since the amount of the element types that could be dynamically instanced is finite and known,
    // we can simply pregenerate a bunch of parameter sets for each possible type that match the properties it needs!
    // Unfortunately, it introduces a limit to how many instances of that given element can be instanced...
    // (though, that can be changed easily and probably shouldn't introduce incompatibilities when loading patches)
    for(const auto [type, params] : param_map)
    {
        for(ElementID i = 1; i <= DYNAMIC_PARAMETER_SET_N; i++)
        {
            const auto id_str = formatID(i);
            for(const ParamInfo& param : params)
            {
                const auto key = getDynamicParameterNameStr(type, param.name, id_str);
                // The param.name may contain underscores... but it's not a major concern, 
                // a better friendly name can be added to the ParamInfo struct without breaking things.
                const auto friendly_name = type + " " + id_str + " " + param.name;
                layout.add(std::make_unique<juce::AudioParameterFloat>(
                    key, friendly_name, param.min, param.max, param.default_value
                ));
            }
        }
    }
    return layout;
}

void SynthStateManager::regenerateElementTrees()
{
    // Add subtrees for the dynamic components of the synth. Warning - never add an undo manager here!
    if(!parameters.state.getChildWithName(Name::GENERATORS).isValid())
        parameters.state.appendChild(juce::ValueTree(Name::GENERATORS), nullptr);
    if(!parameters.state.getChildWithName(Name::PROCESSORS).isValid())
        parameters.state.appendChild(juce::ValueTree(Name::PROCESSORS), nullptr);
    if(!parameters.state.getChildWithName(Name::SINKS).isValid())
        parameters.state.appendChild(juce::ValueTree(Name::SINKS), nullptr);
    if(!parameters.state.getChildWithName(Name::META_STATE).isValid())
        parameters.state.appendChild(juce::ValueTree(Name::META_STATE, {
            {Name::META_NEXT_FREE_DYNAMIC_PARAMETER_ID, 0} // FIXME low priority - if there exist trees with parameters already, then this would be an invalid value. However - that would already be an incorrect state anyway.
        }), nullptr);

    meta.generators = parameters.state.getChildWithName(Name::GENERATORS);
    jassert(meta.generators.isValid());
    meta.processors = parameters.state.getChildWithName(Name::PROCESSORS);
    jassert(meta.processors.isValid());
    meta.sinks = parameters.state.getChildWithName(Name::SINKS);
    jassert(meta.sinks.isValid());
}

size_t SynthStateManager::getNextDynamicParameterID() const
{
    return size_t(juce::int64(parameters.state.getChildWithName(Name::META_STATE)[Name::META_NEXT_FREE_DYNAMIC_PARAMETER_ID]));
}

void SynthStateManager::setNextFreeDynamicParameterID(const size_t id)
{
    parameters.state.getChildWithName(Name::META_STATE).setProperty(Name::META_NEXT_FREE_DYNAMIC_PARAMETER_ID, juce::int64(id), nullptr);
}

juce::String getDynamicParameterNameID(const juce::Identifier& type,
    const juce::Identifier& param_name, const ElementID element_id)
{
    return getDynamicParameterNameStr(type, param_name, formatID(element_id));
}

juce::String getDynamicParameterNameStr(const juce::Identifier& type,
    const juce::Identifier& param_name, const juce::StringRef element_id)
{
    return type.toString() + element_id + "." + param_name;
}

juce::String formatID(const ElementID id)
{
    return juce::String::formatted("%03d", std::abs(id));
}

juce::ValueTree SynthStateManager::newOscillatorRep(ElementID id)
{
    jassert(matchesSign(id, SIGN_GENERATOR));
    return juce::ValueTree(Name::OSCILLATOR,
        {
            {Name::ID,             id},
            {Name::INDEX,          -id-1},
            // FIXME Hack - see addElementRep's FIXME
            {Name::RATIO,          getDynamicParameterNameID(Name::OSCILLATOR, Name::RATIO,          id)},
            {Name::STARTING_PHASE, getDynamicParameterNameID(Name::OSCILLATOR, Name::STARTING_PHASE, id)},
            {Name::PULSE_WIDTH,    getDynamicParameterNameID(Name::OSCILLATOR, Name::PULSE_WIDTH,    id)},
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

juce::ValueTree SynthStateManager::newGateRep(ElementID id, const ElementType type)
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

juce::ValueTree SynthStateManager::newBitMixChannelRep(const ElementID id)
{
    jassert(matchesSign(id, SIGN_SINK));
    return juce::ValueTree(Name::MIX_CHANNEL,
        {
            {Name::ID, id},
            {Name::INDEX, -id-1},
            // FIXME Hack - see addElementRep's FIXME
            {Name::LEVEL, getDynamicParameterNameID(Name::MIX_CHANNEL, Name::LEVEL, id)},
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

ElementID SynthStateManager::addElementRep(const ElementCategory element_category, const ElementType gate_type)
{
    juce::ValueTree& element_container = getElementContainer(element_category);
    jassert(element_container.isValid());

    const auto num = element_container.getNumChildren();
    const ElementID new_id = createElementID(num, element_category);

    throwassert(element_container.getChildWithProperty(Name::ID, new_id).isValid() == false,
                std::logic_error("Element with the same ID already exists in the tree"));
    // FIXME Hack - This will be a problem when other sink/generator types are introduced other than the existing ones.
    // Of course, not all sinks/generators may be parametrized, so the condition is hacky...
    // But also, they'll have to share the ElementID space!
    // We'll probably need to keep track of the number of each type of element separately, with a map probably.
    throwassert(element_category == ElementCategory::GENERATOR || element_category == ElementCategory::SINK
                    ? std::abs(new_id) <= DYNAMIC_PARAMETER_SET_N : true,
                LimitError("The maximum number of the given element type instances has already been reached"));

    juce::ValueTree new_tree;
    switch(element_category)
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

