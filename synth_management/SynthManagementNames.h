//
// Created by micha on 24.11.2023.
//
#pragma once


// ID standard:
// == 0 = none
//  > 0 = Gate
//  < 0 = Oscillator
typedef int ConnectionID;

enum class ElementType
{
    OSCILLATOR,
    GATE,
    MIX_CHANNEL,
};

enum class GateType
{
    NONE, // Only exists so non-gate ElementAdderButtons don't need to be assigned a gate type
    NOT,
    AND, OR, XOR,
};


