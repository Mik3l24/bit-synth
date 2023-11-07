//
// Created by micha on 06.11.2023.
//

#include "GateNode.h"

status GateNode::processBlock()
{
    // Should this be checked in processBlock?
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
    return status::SUCCESS;
}



UnaryGate::UnaryGate()
{
    inputs.assign({nullptr});
}

BinaryGate::BinaryGate()
{
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



