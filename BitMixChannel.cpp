//
// Created by micha on 07.11.2023.
//
#include "BitMixChannel.h"

#include <iostream>

float BitMixChannel::getSample(int sample_index)
{
    // Should be skipped if not connected, but just in case
    if(checkConnections() == status::UNCONNECTED)
        return 0.0f;

    std::cout << "sample : " << inputs[0]->getOut()[sample_index] << '\n'; //Debug
    // Currently 0 to level, could alternatively be -level/2 to level/2
    return float(bool(inputs[0]->getOut()[sample_index])) * level;
}


BitMixChannel::BitMixChannel()
{
    num_inputs = 1;
    inputs.assign({nullptr});
}
