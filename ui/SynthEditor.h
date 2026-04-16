//
// Created by micha on 12.04.2026.
//

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#include "TopBar.h"
#include "StructureEditor.h"
#include "synth_management/SynthStateManager.h"

namespace ui
{
    class SynthEditor : public juce::AudioProcessorEditor,
                        public juce::Button::Listener
    {
    public:
        SynthEditor(juce::AudioProcessor& parent, SynthStateManager& state_manager);

        void resized() override;

        void buttonClicked(juce::Button*) override;

        void saveStateToFile(const juce::File& file);
        void loadStateFromFile(const juce::File& file);



    private:
        SynthStateManager state_manager;

        std::unique_ptr<TopBar> top_bar;
        std::unique_ptr<StructureEditor> structure_editor;

    };
}
