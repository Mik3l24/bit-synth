//
// Created by micha on 30.11.2023.
//
#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace ui
{

class Theme
{
public:
    enum class Scheme
    {
        DARK, // Generally dark grays
        LIGHT_AND_DARK, // Generally light, but with some dark accent elements
        LIGHT, // Generally light
    };

public:
    Theme() = delete;

public:
    static juce::Colour getStructureBackground()
    {
        switch(scheme)
        {
            case Scheme::DARK:
                return juce::Colour(0xff505050);
            case Scheme::LIGHT_AND_DARK:
            case Scheme::LIGHT:
                return juce::Colour(0xffe0e0e0);
        }
    }

    static juce::Colour getStructureLogicForeground()
    {
        switch(scheme)
        {
            case Scheme::DARK:
                return juce::Colour(0xffe0e0e0);
            case Scheme::LIGHT_AND_DARK:
            case Scheme::LIGHT:
                return juce::Colour(0xff202020);
        }
    }

    static float getStructureLogicStrokeThickness()
    {
        return 5.0f;
    }

private:
    static Scheme scheme;
};

} // ui


