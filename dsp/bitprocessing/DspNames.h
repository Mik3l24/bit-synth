//
// Created by micha on 06.11.2023.
//
#pragma once
#include <boost/dynamic_bitset.hpp>


typedef boost::dynamic_bitset<> bitset;

//typedef int status;

enum class status
{
    SUCCESS = 0,
    POSTPONED,
    UNCONNECTED,
};


