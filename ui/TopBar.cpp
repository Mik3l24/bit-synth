//
// Created by micha on 10.04.2026.
//

#include "TopBar.h"


namespace ui
{
    TopBar::TopBar(juce::Button::Listener* listener)
    {
        save_button = std::make_unique<juce::TextButton>("Save");
        addAndMakeVisible(save_button.get());
        save_button->addListener(listener);

        save_button->setBounds(0, 0, 48, 20);

        load_button = std::make_unique<juce::TextButton>("Load");
        addAndMakeVisible(load_button.get());
        load_button->addListener(listener);

        load_button->setBounds(50, 0, 48, 20);

    }

    TopBar::~TopBar()
    {
    }

    void TopBar::paint(juce::Graphics& g)
    {

    }

    void TopBar::resized()
    {

    }
} // ui