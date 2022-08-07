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
    void timerCallback() override;
public:
    PianoRollComponent(MidiKeyboardState& state, KeyboardComponent::Orientation orientation);
    void paint (juce::Graphics&) override;
    void resized() override;
    void mouseWheelMove(const MouseEvent &event, const MouseWheelDetails &wheel) override;
};

class PianoRollItem: public foleys::GuiItem {
private:
    PianoRollComponent pianoRoll;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PianoRollItem)
public:
    FOLEYS_DECLARE_GUI_FACTORY(PianoRollItem)

    PianoRollItem(foleys::MagicGUIBuilder& builder, const juce::ValueTree& node): foleys::GuiItem(builder, node),
                                                    pianoRoll(getMagicState().getKeyboardState(), juce::MidiKeyboardComponent::verticalKeyboardFacingRight) {
        addAndMakeVisible(pianoRoll);
    }

    void update() override {

    }

    juce::Component* getWrappedComponent() override {
        return &pianoRoll;
    }
};