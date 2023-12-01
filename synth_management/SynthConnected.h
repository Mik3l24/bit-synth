//
// Created by micha on 24.11.2023.
//
#pragma once

#include "BitSynthesizer.h"

class SynthConnected
{
public:
//    SynthConnected() = default;
    SynthConnected(BitSynthesizer* synth)
        : synth(synth) {}

//    void setSynth(BitSynthesizer* _synth)
//    {
//        synth = _synth;
//    }
protected:
    BitSynthesizer* synth = nullptr;
};


