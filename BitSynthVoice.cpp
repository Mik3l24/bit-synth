//
// Created by micha on 08.11.2023.
//
#include "BitSynthVoice.h"


bool BitSynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<BitSynthSound*> (sound) != nullptr;
}
