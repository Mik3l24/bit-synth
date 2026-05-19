//
// Created by micha on 06.11.2023.
//

#include "Gate.h"

using namespace dsp;

void Gate::resetStatus()
{
    ready = false;
}

Gate::Gate(const bitset& _zeroes, const SubConnectionID _input_n)
    : BitReceiver(_zeroes, _input_n) {}

void Gate::prepareToPlay(double _sample_rate, int _samples_per_block)
{
    BitReceiver::prepareToPlay(_sample_rate, _samples_per_block);
    BitSource::prepareToPlay(_sample_rate, _samples_per_block);
}


UnaryGate::UnaryGate(const bitset& _zeroes)
    : Gate(_zeroes, 1) {}

BinaryGate::BinaryGate(const bitset& _zeroes)
    : Gate(_zeroes, 2) {}

status NotGate::processBlock()
{
    return processBlockImpl<&bitset::replace_with_not_of>();
}

status AndGate::processBlock()
{
    return processBlockImpl<&bitset::replace_with_and_of>();
}

status OrGate::processBlock()
{
    return processBlockImpl<&bitset::replace_with_or_of>();
}

status XorGate::processBlock()
{
    return processBlockImpl<&bitset::replace_with_xor_of>();
}


