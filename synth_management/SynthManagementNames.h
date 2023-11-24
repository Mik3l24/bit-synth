//
// Created by micha on 24.11.2023.
//
#pragma once


// ID standard:
// == 0 = none
//  > 0 = Gate
//  < 0 = Oscillator
typedef int ConnectionID;


enum class GateType {
    NOT,
    AND, OR, XOR,
};


