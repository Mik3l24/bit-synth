//
// Created by micha on 06.11.2023.
//
#pragma once
#include "BitIO.h"

namespace dsp
{

class Gate : public BitSource, public BitReceiver
{
public:
    explicit Gate(const bitset& _zeroes, SubConnectionID _input_n = 1);

public:
    void prepareToPlay(double _sample_rate, int _samples_per_block) override;
    virtual status processBlock() = 0;
    void resetStatus();
};


class UnaryGate : public Gate
{
public:
    explicit UnaryGate(const bitset& _zeroes);

protected:
    template<void (bitset::*operation)(const bitset&)>
    forcedinline status processBlockImpl()
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

        (out.*operation)(getOutFromInput());
        ready = true; // How did I forget this?
        return status::SUCCESS;
    }
};

class BinaryGate : public Gate
{
public:
    explicit BinaryGate(const bitset& _zeroes);

protected:
    template<void (bitset::*operation)(const bitset&, const bitset&)>
    forcedinline status processBlockImpl()
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

        (out.*operation)(getOutFromInput(0), getOutFromInput(1));
        ready = true; // How did I forget this?
        return status::SUCCESS;
    }
};

class NotGate : public UnaryGate
{
public:
    explicit NotGate(const bitset& _zeroes)
      : UnaryGate(_zeroes) {};

    status processBlock() override;
};

class AndGate : public BinaryGate
{
public:
    explicit AndGate(const bitset& _zeroes)
      : BinaryGate(_zeroes) {};

    status processBlock() override;
};

class OrGate : public BinaryGate
{
public:
    explicit OrGate(const bitset& _zeroes)
      : BinaryGate(_zeroes) {};

    status processBlock() override;
};

class XorGate : public BinaryGate
{
public:
    explicit XorGate(const bitset& _zeroes)
      : BinaryGate(_zeroes) {};

    status processBlock() override;
};

}
