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
    return status::SUCCESS;
}