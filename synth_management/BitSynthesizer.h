//
// Created by micha on 15.11.2023.
//
#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include <vector>

#include "RepresentationStructs.h"
#include "SynthManagementNames.h"


class BitSynthesizer : public juce::Synthesiser
{
public: // constructors
    explicit BitSynthesizer(int num_voices);

protected: // Utility access methods
    juce::ValueTree getOscillator(ConnectionID id) const;

    juce::ValueTree getGate(ConnectionID id) const;

    juce::ValueTree getMixChannel(ConnectionID id) const;

    juce::ValueTree getSource(ConnectionID id) const;

public: // Voice parameter access methods // TODO: removal methods
    // Oscillators
    ConnectionID addOscillator();
    void setOscillatorRatio(ConnectionID id, double ratio);
    void setOscillatorStartingPhase(ConnectionID id, double starting_phase);
    void setOscillatorPulseWidth(ConnectionID id, float pulse_width);
    // Gates
    ConnectionID addGate(GateType type);
    void setGateInput(ConnectionID id, ConnectionID input_id, size_t input_index);
    // Mix channels
    ConnectionID addMixChannel();
    void setMixChannelInput(ConnectionID id, ConnectionID input_id);
    void setMixChannelLevel(ConnectionID id, float level);
    // Other
    void setMasterLevel(float level);

    std::unique_ptr<juce::XmlElement> toXml() const;
    void fromXml(const juce::XmlElement& xml);



protected: // parameter members
#if false
    std::vector<OscillatorRepresentation> oscillators;
    std::vector<GateNodeRepresentation> gates;
    std::vector<BitMixChannelRepresentation> mix_channels;
#endif
    // Other per-voice parameters
    float master_level = .125f;

};


