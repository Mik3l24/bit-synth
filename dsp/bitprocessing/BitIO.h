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
public: // Constructors, destructors
    virtual ~Connectable() = default;

public: // DSP setup and processing methods
    virtual void prepareToPlay(double _sample_rate, int _samples_per_block)
    {
        juce::ignoreUnused(_sample_rate, _samples_per_block);
    }
};

class BitSource : public Connectable
{
public: // DSP setup and processing methods
    void prepareToPlay(double _sample_rate, int _samples_per_block) override;
    [[nodiscard]] const bitset& getOut() const { return out; }
    [[nodiscard]] bool isReady() const { return ready; }

protected:
    bitset out;
    bool ready = false;
};

class BitReceiver : public Connectable
{
public: // Constructors, destructors
    explicit BitReceiver(const bitset& _zeroes, SubConnectionID _num_inputs = 1);

public: // DSP setup and processing methods
    void prepareToPlay(double _sample_rate, int _samples_per_block) override;

public: // Parameter setters, getters
    void setInput(const BitSource* input, SubConnectionID index = 0);

protected: // Internal interfaces for subclasses
    status checkConnections();
    [[nodiscard]] const bitset& getOutFromInput(SubConnectionID index = 0) const;

protected: // Fields
    const SubConnectionID num_inputs = 1;

private: // Fields
    std::vector<const BitSource*> inputs;

    int num_samples = 0;

    /** Bitset containing only zeroes, used for unconnected inputs.
     *
     *  Size is equal to num_samples, which should be the same as the block size.
     */
    const bitset& zeroes;
};

}
