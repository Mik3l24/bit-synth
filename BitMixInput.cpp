//
// Created by micha on 07.11.2023.
//
#include "BitMixInput.h"


float BitMixInput::getSample(int sample_index)
{
    // Should be skipped if not connected, but just in case
    if(checkConnections() == status::UNCONNECTED)
        return 0.0f;

    // Currently 0 to level, could alternatively be -level/2 to level/2
    return (float)(inputs[0]->getOut()[sample_index]) * level;
}


BitMixInput::BitMixInput()
{
    inputs.assign({nullptr});
}