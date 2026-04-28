//
// Created by micha on 24.11.2023.
//
#pragma once

#include <juce_core/juce_core.h>
// ID standard:
// == 0 = none
//  > 0 = Gate
//  < 0 = Oscillator
//  < 0 = Sink (new)
typedef juce::int64 ConnectionID;
typedef bool ConnectionSign;
typedef int32_t ElementID;
typedef int32_t SubConnectionID;

constexpr ConnectionID CONNECTION_NONE = 0;
constexpr ConnectionSign SIGN_GENERATOR = true;
constexpr ConnectionSign SIGN_PROCESSOR = false;
constexpr ConnectionSign SIGN_SINK = true;

namespace Name
{
    const juce::Identifier ROOT("BitSynth");

    // Container subtree names
    const juce::Identifier GENERATORS("Generators");
    const juce::Identifier PROCESSORS("Processors");
    const juce::Identifier SINKS     ("Sinks");
    const juce::Identifier META_STATE("MetaState");

    // Element subtree names
    const juce::Identifier OSCILLATOR("Oscillator");

    const juce::Identifier GATE_OR  ("GateOR");
    const juce::Identifier GATE_AND ("GateAND");
    const juce::Identifier GATE_NOT ("GateNOT");
    const juce::Identifier GATE_XOR ("GateXOR");

    const juce::Identifier MIX_CHANNEL ("MixChannel");

    const juce::Identifier CONNECTION("Connection");
    const juce::Identifier CONNECTIONS("Connections");

    // Parameter names
    const juce::Identifier ID("id");
    const juce::Identifier INDEX("index");
    const juce::Identifier RATIO("ratio");
    const juce::Identifier STARTING_PHASE("starting_phase");
    const juce::Identifier PULSE_WIDTH("pulse_width");

    const juce::Identifier INPUT_N("input_n");

    const juce::Identifier LEVEL("level");

    const juce::Identifier MASTER_LEVEL("master_level");

    const juce::Identifier META_UI_POSITION_X("__ui_position_x");
    const juce::Identifier META_UI_POSITION_Y("__ui_position_y");
    const juce::Identifier META_NEXT_FREE_DYNAMIC_PARAMETER_ID("next_free_dynamic_parameter_id");

}

enum class ElementCategory
{
    GENERATOR,
    PROCESSOR,
    SINK,
};


enum class GateType
{
    NONE, // Only exists so non-gate ElementAdderButtons don't need to be assigned a gate type
    NOT,
    AND, OR, XOR,
};


namespace __SynthManagementNames_private // NOLINT(*-reserved-identifier)
{
    constexpr juce::uint64 ELEMENT_MASK = 0x00000000FFFFFFFF;
    constexpr juce::uint64 INDEX_MASK   = 0xFFFFFFFF00000000;
    constexpr auto INDEX_SHIFT  = 32;
}

inline ConnectionSign isNegative(const ConnectionID connection)
{
    return connection < 0;
}

inline ConnectionID negate(const ConnectionID connection)
{
    return -connection;
}

inline ConnectionID applySign(const ConnectionID connection, const ConnectionSign sign)
{
    return sign ? negate(connection) : connection;
}

inline bool matchesSign(const ConnectionID connection, const ConnectionSign sign)
{
    return connection != 0 && isNegative(connection) == sign;
}

inline bool matchesSign(const ElementID element_id, const ConnectionSign sign)
{
    return element_id != 0 && (element_id < 0) == sign;
}

inline ElementID toElementID(const ConnectionID id)
{
    using namespace __SynthManagementNames_private;
    const auto abs = std::abs(id);
    const auto masked = abs & ELEMENT_MASK;
    return isNegative(id) ? -ElementID(masked) : ElementID(masked);
}

inline SubConnectionID toSubConnectionID(const ConnectionID id)
{
    using namespace __SynthManagementNames_private;
    const auto abs = std::abs(id);
    const auto masked = abs & INDEX_MASK;
    return SubConnectionID(masked >> INDEX_SHIFT);
}

inline ConnectionID appendSubConnectionID(const SubConnectionID connection, const ConnectionID id)
{
    using namespace __SynthManagementNames_private;
    const ConnectionSign sign = isNegative(connection);
    const auto abs = std::abs(id);
    const auto masked = abs & ELEMENT_MASK; // To clear previous connection
    const auto appended = masked | ConnectionID(connection) << INDEX_SHIFT;
    return applySign(appended, sign);
}

inline ConnectionID createConnectionID(ElementID element_id, const SubConnectionID sub_connection_id, const ConnectionSign sign)
{
    using namespace __SynthManagementNames_private;
    element_id = std::abs(element_id);
    const auto masked_element_id = juce::uint64(element_id) & ELEMENT_MASK;
    const auto masked_sub_connection_id = (juce::uint64(sub_connection_id) << INDEX_SHIFT) & INDEX_MASK;
    const auto combined = masked_element_id | masked_sub_connection_id;
    return applySign(ConnectionID(combined), sign);
}

inline ElementID createElementID(const int index, const ElementCategory element_type)
{
    switch(element_type)
    {
        case ElementCategory::GENERATOR: return -ElementID(index + 1);
        case ElementCategory::PROCESSOR: return ElementID(index + 1);
        case ElementCategory::SINK:      return -ElementID(index + 1);
        default: jassertfalse; return 0;
    }
}

inline juce::int32 getElementIndex(const ElementID element_id)
{
    jassert(element_id != 0);
    return std::abs(element_id) - 1;
}

inline std::tuple<ElementID, SubConnectionID, ConnectionSign> decodeConnectionID(const ConnectionID connection_id)
{
    return std::make_tuple(toElementID(connection_id), toSubConnectionID(connection_id), isNegative(connection_id));
}

inline const juce::Identifier& toContainerIdentifier(const ElementCategory element_type)
{
    switch(element_type)
    {
        case ElementCategory::GENERATOR: return Name::GENERATORS;
        case ElementCategory::PROCESSOR: return Name::PROCESSORS;
        case ElementCategory::SINK:      return Name::SINKS;
        default: jassertfalse; return juce::Identifier();
    }
}

inline const juce::Identifier& toGateIdentifier(const GateType gate_type)
{
    switch(gate_type)
    {
        case GateType::NOT: return Name::GATE_NOT;
        case GateType::AND: return Name::GATE_AND;
        case GateType::OR:  return Name::GATE_OR;
        case GateType::XOR: return Name::GATE_XOR;
        default: jassertfalse; return juce::Identifier();
    }
}

inline GateType toGateEnum(const juce::Identifier& gate_identifier)
{
    if(gate_identifier == Name::GATE_NOT) return GateType::NOT;
    if(gate_identifier == Name::GATE_AND) return GateType::AND;
    if(gate_identifier == Name::GATE_OR)  return GateType::OR;
    if(gate_identifier == Name::GATE_XOR) return GateType::XOR;
    jassertfalse; return GateType::NONE;
}


inline int gateMaxInputN(const GateType gate_type)
{
    switch(gate_type)
    {
        case GateType::NOT: return 1;
        case GateType::AND:
        case GateType::OR:
        case GateType::XOR: return 2;
        default: jassertfalse; return 0;
    }
}

inline bool isIdentifierAGate(const juce::Identifier& id)
{
    return id == Name::GATE_NOT || id == Name::GATE_AND || id == Name::GATE_OR || id == Name::GATE_XOR;
}

inline bool isIdentifierASink(const juce::Identifier& id)
{
    return id == Name::MIX_CHANNEL;
}


