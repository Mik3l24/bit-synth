//
// Created by micha on 06.11.2023.
//
#pragma once
#include "boost/dynamic_bitset.hpp"

namespace dsp
{

typedef boost::dynamic_bitset<uint64_t> bitset;

//typedef int status;

enum class status
{
    SUCCESS = 0,
    POSTPONED,
    UNCONNECTED,
};

}
