//
// Created by Andrei Gheorghe on 07.08.2022.
//

#include "KeyboardComponent.h"

KeyboardComponent::KeyboardComponent(MidiKeyboardState& state, Orientation orientation): MidiKeyboardComponent(state, orientation) {
    setColour(keySeparatorLineColourId, Colours::transparentWhite);
}

Range<float> KeyboardComponent::getKeyPosition(int midiNoteNumber, float targetKeyWidth) const {
    jassert(midiNoteNumber >= 0 && midiNoteNumber < 128);
    static const float notePos[] = { 0.0f, 1,
                                     1.5f, 3,
                                     3.5f,
                                     5.0f, 6,
                                     6.5f, 8,
                                     8.5f, 10,
                                     10.5f };

    auto octave = midiNoteNumber / 12;
    auto note = midiNoteNumber % 12;

    auto start = static_cast<float>(octave) * 12.0f * targetKeyWidth + notePos[note] * targetKeyWidth;
    auto width = targetKeyWidth;

    return { start, start + width };
}

Range<float> KeyboardComponent::getKeyPos (int midiNoteNumber) const
{
    return getKeyPosition (midiNoteNumber, keyWidth)
           - xOffset
           - getKeyPosition (rangeStart, keyWidth).getStart();
}

Rectangle<float> KeyboardComponent::getRectangleForKey(int note) const {
    jassert(note >= rangeStart && note <= rangeEnd);

    auto pos = getKeyPos(note);
    auto x = pos.getStart();
    auto w = pos.getLength();

    if (MidiMessage::isMidiNoteBlack(note))
    {
        auto blackNoteLength = getBlackNoteLength();

        switch (getOrientation())
        {
            case horizontalKeyboard:            return { x, 0, w, blackNoteLength };
            case verticalKeyboardFacingLeft:    return {static_cast<float>(getWidth()) - blackNoteLength, x, blackNoteLength, w };
            case verticalKeyboardFacingRight:   return {0, static_cast<float>(getHeight()) - x - w, blackNoteLength, w };
            default:                            jassertfalse; break;
        }
    }
    else
    {
        w *= getKeyWidthModifier(note);
        switch (getOrientation())
        {
            case horizontalKeyboard:            return { x, 0, w, (float)getHeight() };
            case verticalKeyboardFacingLeft:    return { 0, x, (float)getWidth(), w };
            case verticalKeyboardFacingRight:   return {0, static_cast<float>(getHeight()) - x - w, (float)getWidth(), w };
            default:                            jassertfalse; break;
        }
    }

    return {};
}

float KeyboardComponent::getKeyWidthModifier(int midiNoteNumber)
{
    auto name = MidiMessage::getMidiNoteName(midiNoteNumber, true, false, 3);

    if (name == "C" || name == "E" || name == "F" || name == "B")
        return 1.5f;
    if (name == "D" || name == "G" || name == "A")
        return 2;

    return 1;
}

float KeyboardComponent::getFirstKey() {
    return this->firstKey;
}

String KeyboardComponent::getWhiteNoteText(int midiNoteNumber) {
    return MidiMessage::getMidiNoteName (midiNoteNumber, true, true, getOctaveForMiddleC());
}
