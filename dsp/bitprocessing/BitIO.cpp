//
// Created by micha on 07.11.2023.
//
#include "BitIO.h"


status BitReceiver::checkConnections()
{
    for(auto in: inputs)
    {
        if(in == nullptr
           || in->isUnconnected())
        {
            unconnected = true;
            return status::UNCONNECTED;
        }
    }
    unconnected = false;
    return status::SUCCESS;
}

void BitReceiver::setInput(const BitSource* input, int index)
{
    assert(index < num_inputs);
    inputs[index] = input;
    unconnected = false; // FIXME: Does not propagate to children
}
