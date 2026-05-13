//
// Created by micha on 07.11.2023.
//
#include "BitIO.h"
#include <juce_core/juce_core.h>

using namespace dsp;

void BitSource::prepareToPlay(double _sample_rate, int _samples_per_block)
{
    juce::ignoreUnused(_sample_rate);
    out.resize(size_t(_samples_per_block), 0);
}

BitReceiver::BitReceiver(const bitset& _zeroes, const SubConnectionID _num_inputs)
    : num_inputs(_num_inputs), zeroes(_zeroes)
{
    inputs.assign(size_t(_num_inputs), nullptr);
}

void BitReceiver::prepareToPlay(double _sample_rate, const int _samples_per_block)
{
    juce::ignoreUnused(_sample_rate);
    num_samples = _samples_per_block;
}

// Not really necessary for checking for being unconnected now that getOutFromInput() handles that...
// But with inputs being a private member now, it's the only way to check for readiness
status BitReceiver::checkConnections()
{
    for(auto in: inputs)
    {
        if(in == nullptr)
        {
            return status::UNCONNECTED;
        }
        else if(!in->isReady())
        {
            return status::POSTPONED;
        }
    }
    return status::SUCCESS;
}

const bitset& BitReceiver::getOutFromInput(SubConnectionID index) const
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

void BitReceiver::setInput(const BitSource* input, SubConnectionID index)
{
    jassert(index < num_inputs);
    inputs[index] = input;
}
