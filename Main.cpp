/*
  ==============================================================================

    This file contains the startup code for a PIP.

  ==============================================================================
*/

#include <juce_audio_processors/juce_audio_processors.h>
#include "SynthAudioProcessor.h"

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SynthAudioProcessor();
}

