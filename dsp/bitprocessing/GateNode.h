//
// Created by micha on 06.11.2023.
//
#pragma once
#include "BitIO.h"



class GateNode : public BitSource, public BitReceiver
{
public:
    GateNode(size_t num_inputs = 1);

public:
    status processBlock();
    void resetStatus();

private:
    // Called in processBlock() if all inputs are ready
    virtual bitset operation() = 0;

};

class UnaryGate : public GateNode
{
public:
    UnaryGate();
};

class BinaryGate : public GateNode
{
public:
    BinaryGate();
};

class NotGate : public UnaryGate
{
private:
    bitset operation() override;
};

class AndGate : public BinaryGate
{
private:
    bitset operation() override;
};

class OrGate : public BinaryGate
{
private:
    bitset operation() override;
};

class XorGate : public BinaryGate
{
private:
    bitset operation() override;
};
