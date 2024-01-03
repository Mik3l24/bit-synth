//
// Created by micha on 06.11.2023.
//

#include "GateNode.h"

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

GateNode::GateNode(size_t num_inputs)
    : BitReceiver(num_inputs) {}


UnaryGate::UnaryGate()
    : GateNode(1) {}

BinaryGate::BinaryGate()
    : GateNode(2) {}


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



