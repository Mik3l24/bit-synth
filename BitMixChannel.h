//
// Created by micha on 07.11.2023.
//
#pragma once
#include "BitIO.h"


// Always one input
class BitMixChannel : public BitReceiver
{
public:
    float getSample(int sample_index);

public:
    BitMixChannel();

protected: // Parameters
    float level = .125f;
};



