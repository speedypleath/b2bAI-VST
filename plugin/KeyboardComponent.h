//
// Created by Andrei Gheorghe on 07.08.2022.
//
#pragma once

#include "JuceHeader.h"

class KeyboardComponent: public MidiKeyboardComponent {
public:
    KeyboardComponent(MidiKeyboardState& state, Orientation orientation);
    Range<float> getKeyPosition(int midiNoteNumber, float targetKeyWidth) const override;
    Rectangle<float> getRectangleForKey(int note) const override;
    Range<float> getKeyPos (int midiNoteNumber) const;
    static float getKeyWidthModifier(int midiNoteNumber);
    String getWhiteNoteText (int midiNoteNumber) override ;
    float getFirstKey();
};