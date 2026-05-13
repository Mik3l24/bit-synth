//
// Created by micha on 06.11.2023.
//
#pragma once
#include "BitIO.h"

namespace dsp
{

class GateNode : public BitSource, public BitReceiver
{
public:
    GateNode(const bitset& _zeroes, SubConnectionID _input_n = 1);

public:
    void prepareToPlay(double _sample_rate, int _samples_per_block) override;
    status processBlock();
    void resetStatus();

private:
    // Called in processBlock() if all inputs are ready
    virtual bitset operation() = 0;

};

class UnaryGate : public GateNode
{
public:
    explicit UnaryGate(const bitset& _zeroes);
};

class BinaryGate : public GateNode
{
public:
    explicit BinaryGate(const bitset& _zeroes);
};

class NotGate : public UnaryGate
{
public:
    explicit NotGate(const bitset& _zeroes)
      : UnaryGate(_zeroes) {};
private:
    bitset operation() override;
};

class AndGate : public BinaryGate
{
public:
    explicit AndGate(const bitset& _zeroes)
      : BinaryGate(_zeroes) {};
private:
    bitset operation() override;
};

class OrGate : public BinaryGate
{
public:
    explicit OrGate(const bitset& _zeroes)
      : BinaryGate(_zeroes) {};
private:
    bitset operation() override;
};

class XorGate : public BinaryGate
{
public:
    explicit XorGate(const bitset& _zeroes)
      : BinaryGate(_zeroes) {};
private:
    bitset operation() override;
};

}
