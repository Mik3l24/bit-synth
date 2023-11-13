//
// Created by micha on 06.11.2023.
//
#pragma once
#include "Declarations.h"
#include <vector>

class Connectable
{
public:
    [[nodiscard]] bool isUnconnected() const { return unconnected; }
public: // Constructors, destructors
    virtual ~Connectable() = default;
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
    void setInput(const BitSource* input, int index = 0);

protected:
    std::vector<const BitSource*> inputs;
    std::size_t num_inputs = 1;
};

