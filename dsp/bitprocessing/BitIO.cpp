//
// Created by micha on 07.11.2023.
//
#include "BitIO.h"
#include <juce_core/juce_core.h>


BitReceiver::BitReceiver(size_t num_inputs)
    : num_inputs(num_inputs)
{
    inputs.assign(num_inputs, nullptr);
}

void BitReceiver::prepareToPlay(size_t samplesPerBlockExpected)
{
    num_samples = samplesPerBlockExpected;
    zeroes = bitset(num_samples);
}

// Not really necessary for checking for being unconnected now that getOutFromInput() handles that...
// But with inputs being a private member now, it's the only way to check for readiness
status BitReceiver::checkConnections()
{
    for(auto in: inputs)
    {
        if(in == nullptr)
        {
            unconnected = true;
            return status::UNCONNECTED;
        }
        else if(!in->isReady())
        {
            return status::POSTPONED;
        }
    }
    unconnected = false;
    return status::SUCCESS;
}

const bitset& BitReceiver::getOutFromInput(size_t index) const
{
    // getOut() and getOutFromInput() used to have to construct a new bitset every call,
    // which wasn't ideal to be called for every sample, like in BitMixChannel.
    // So, now they return a const reference.
    jassert(index < num_inputs);
    if(inputs[index] != nullptr)
        return inputs[index]->getOut();
    else
        return zeroes;
}

void BitReceiver::setInput(const BitSource* input, size_t index)
{
    jassert(index < num_inputs);
    inputs[index] = input;
    unconnected = false; // FIXME: Does not propagate to children
}
