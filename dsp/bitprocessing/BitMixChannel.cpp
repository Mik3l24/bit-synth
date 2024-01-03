//
// Created by micha on 07.11.2023.
//
#include "BitMixChannel.h"

float BitMixChannel::getSample(int sample_index)
{
    // Should be skipped if not connected, but just in case
    if(checkConnections() == status::UNCONNECTED)
        return 0.0f;

    // Currently 0 to level, could alternatively be -level/2 to level/2
    return float(getOutFromInput()[sample_index]) * level;
}


BitMixChannel::BitMixChannel()
    : BitReceiver(1) {}
