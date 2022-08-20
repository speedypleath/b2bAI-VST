//
// Created by Andrei Gheorghe on 07.08.2022.
//
#pragma once

#include "JuceHeader.h"
#include "GridComponent.h"
#include "KeyboardComponent.h"

class PianoRollComponent: public Component, private juce::Timer {
private:
    std::unique_ptr<KeyboardComponent> keyboardComponent;
    std::unique_ptr<GridComponent> gridComponent;
    MidiSequence *midiSequence = nullptr;
    void timerCallback() override;
public:
    enum ColourIds {
        noteColour = 0x00FF00
    };

    PianoRollComponent(MidiKeyboardState& state, KeyboardComponent::Orientation orientation);
    void paint (juce::Graphics&) override;
    void resized() override;
    void mouseWheelMove(const MouseEvent &event, const MouseWheelDetails &wheel) override;
    void setMidiSequence(MidiSequence *sequence);
};