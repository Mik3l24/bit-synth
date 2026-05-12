//
// Created by micha on 06.11.2023.
//
#pragma once
#include <vector>

#include "DspNames.h"
#include "synth_management/SynthManagementNames.h"


namespace dsp
{

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
    [[nodiscard]] const bitset& getOut() const { return out; }
    [[nodiscard]] bool isReady() const { return ready; }

protected:
    bitset out;
    bool ready = false;
};

class BitReceiver : public virtual Connectable
{
public: // Constructors, destructors
    explicit BitReceiver(size_t num_inputs = 1);

public: // DSP setup and processing methods
    virtual void prepareToPlay(size_t _samples_per_block);

public: // Parameter setters, getters
    void setInput(const BitSource* input, SubConnectionID index = 0);

protected: // Internal interfaces for subclasses
    status checkConnections();
    [[nodiscard]] const bitset& getOutFromInput(SubConnectionID index = 0) const; // TODO: change to const bitset& to avoid copying

protected: // Fields
    const std::size_t num_inputs = 1;

private: // Fields
    std::vector<const BitSource*> inputs;

    size_t num_samples = 0;

    /** Bitset containing only zeroes, used for unconnected inputs.
     *
     *  Size is equal to num_samples, which should be the same as the block size.
     *  @see BitReceiver::prepareToPlay()
     *
     *  Might be modified to be static, since all BitReceivers will very likely be using the same block size anyway.
     */
    bitset zeroes;
};

}
