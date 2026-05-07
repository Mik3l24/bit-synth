//
// Created by mik3l on 7.05.2026.
//

#pragma once
#include <string_view>


// GENERATED: Github Copilot, Claude Haiku 4.5 0.3x
constexpr int getMidiNoteNumberFromName(std::string_view noteName) noexcept
{
    // Mapowanie nut (C=0, D=2, E=4, F=5, G=7, A=9, B=11)
    auto getNoteValue = [](char note) constexpr -> int {
        switch (note) {
        case 'C': case 'c': return 0;
        case 'D': case 'd': return 2;
        case 'E': case 'e': return 4;
        case 'F': case 'f': return 5;
        case 'G': case 'g': return 7;
        case 'A': case 'a': return 9;
        case 'B': case 'b': return 11;
        default: return -1;
        }
    };

    if (noteName.empty()) return -1;

    int noteValue = getNoteValue(noteName[0]);
    if (noteValue == -1) return -1;

    int idx = 1;
    int semitoneAdjustment = 0;

    // Sprawdzenie # lub b
    if (idx < noteName.size()) {
        if (noteName[idx] == '#') {
            semitoneAdjustment = 1;
            idx++;
        } else if (noteName[idx] == 'b' || noteName[idx] == 'B') {
            semitoneAdjustment = -1;
            idx++;
        }
    }

    // Parsowanie numeru oktawy
    int octave = 0;
    bool hasOctave = false;
    while (idx < noteName.size()) {
        char c = noteName[idx];
        if (c >= '0' && c <= '9') {
            octave = octave * 10 + (c - '0');
            hasOctave = true;
            idx++;
        } else if (c == '-' && !hasOctave && octave == 0) {
            // Obsługa liczb ujemnych
            idx++;
            int negOctave = 0;
            while (idx < noteName.size() && noteName[idx] >= '0' && noteName[idx] <= '9') {
                negOctave = negOctave * 10 + (noteName[idx] - '0');
                idx++;
            }
            octave = -negOctave;
            hasOctave = true;
            break;
        } else {
            return -1; // Nieprawidłowy znak
        }
    }

    if (!hasOctave) octave = 4;

    return noteValue + (octave + 1) * 12 + semitoneAdjustment;
}
