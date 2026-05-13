//
// Created by micha on 06.11.2023.
//

#include "GateNode.h"

using namespace dsp;

status GateNode::processBlock()
{
    switch(checkConnections())
    {
        // Being unconnected is now handled by BitReceiver::getOutFromInput(),
        // so the gate doesn't have to terminate processing anymore
        //case status::UNCONNECTED:
        //    return status::UNCONNECTED;
        case status::POSTPONED:
            ready = false;
            return status::POSTPONED;
        default:
            break;
    }

    out = operation();
    ready = true; // How did I forget this?
    return status::SUCCESS;
}

void GateNode::resetStatus()
{
    ready = false;
}

GateNode::GateNode(const bitset& _zeroes, const SubConnectionID _input_n)
    : BitReceiver(_zeroes, _input_n) {}

void GateNode::prepareToPlay(double _sample_rate, int _samples_per_block)
{
    BitReceiver::prepareToPlay(_sample_rate, _samples_per_block);
    BitSource::prepareToPlay(_sample_rate, _samples_per_block);
}


UnaryGate::UnaryGate(const bitset& _zeroes)
    : GateNode(_zeroes, 1) {}

BinaryGate::BinaryGate(const bitset& _zeroes)
    : GateNode(_zeroes, 2) {}


// Okay, I think these guys may actually be allocating a new bitset each time they're called, rather than setting the existing one?
// I may need to refactor how gate polymorphism works. Maybe just have a separate processBlock() for each?
// Though, that may still not modify the out in-place...
bitset NotGate::operation()
{
    return ~(getOutFromInput());
}

bitset AndGate::operation()
{
    return getOutFromInput(0) & getOutFromInput(1);
}

bitset OrGate::operation()
{
    return getOutFromInput(0) | getOutFromInput(1);
}

bitset XorGate::operation()
{
    return getOutFromInput(0) ^ getOutFromInput(1);
}



