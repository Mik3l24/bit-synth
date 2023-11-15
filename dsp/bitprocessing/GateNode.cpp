//
// Created by micha on 06.11.2023.
//

#include "GateNode.h"

status GateNode::processBlock()
{
    // Should this be checked in processBlock?
    // Could be later moved to the construction of the voices from the GUI
    if(checkConnections() == status::UNCONNECTED)
        return status::UNCONNECTED;

    for(auto in: inputs)
    {
        if(!in->isReady())
        {
            ready = false;
            return status::POSTPONED;
        }
    }

    out = operation();
    ready = true; // How did I forget this?
    return status::SUCCESS;
}

void GateNode::resetStatus()
{
    ready = false;
}



UnaryGate::UnaryGate()
{
    num_inputs = 1;
    inputs.assign({nullptr});
}

BinaryGate::BinaryGate()
{
    num_inputs = 2;
    inputs.assign({nullptr, nullptr});
}


bitset NotGate::operation()
{
    return ~(inputs[0]->getOut());
}

bitset AndGate::operation()
{
    return inputs[0]->getOut() & inputs[1]->getOut();
}

bitset OrGate::operation()
{
    return inputs[0]->getOut() | inputs[1]->getOut();
}

bitset XorGate::operation()
{
    return inputs[0]->getOut() ^ inputs[1]->getOut();
}



