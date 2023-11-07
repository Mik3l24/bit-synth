//
// Created by micha on 06.11.2023.
//
#pragma once
#include "Declarations.h"
#include <vector>

class Connectable
{
public:
    [[nodiscard]] bool isUnconnected() const { return unconnected }
protected:
    bool unconnected = false;
};

class BitSource : public virtual Connectable
{
public:
    [[nodiscard]] bitset getOut() const { return out; }
    [[nodiscard]] bool isReady() const { return ready; }

protected:
    bitset out;
    bool ready = false;
};

class BitReceiver : public virtual Connectable
{
public:
    status checkConnections();

protected:
    std::vector<const BitSource*> inputs;
};

