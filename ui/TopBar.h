//
// Created by micha on 10.04.2026.
//

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace ui
{
    class TopBar : public juce::Component
    {
    public:
        explicit TopBar(juce::Button::Listener* listener);
        ~TopBar() override;

        void paint(juce::Graphics& g) override;
        void resized() override;
        //void buttonClicked (juce::Button* buttonThatWasClicked) override;



    public:
        std::unique_ptr<juce::Button> save_button;
        std::unique_ptr<juce::Button> load_button;

    private:

        //==============================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TopBar)
    };
} // ui