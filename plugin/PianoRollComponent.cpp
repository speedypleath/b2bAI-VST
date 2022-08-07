//
// Created by Andrei Gheorghe on 07.08.2022.
//

#include "PianoRollComponent.h"


PianoRollComponent::PianoRollComponent(MidiKeyboardState& state, KeyboardComponent::Orientation orientation) {
    keyboardComponent = std::make_unique<KeyboardComponent>(state, orientation);
    gridComponent = std::make_unique<GridComponent>();
    addAndMakeVisible(*keyboardComponent);
    addAndMakeVisible(*gridComponent);
    startTimerHz (30);
}

void PianoRollComponent::mouseWheelMove(const juce::MouseEvent &event, const juce::MouseWheelDetails &wheel) {
    auto amount = (keyboardComponent->getOrientation() == MidiKeyboardComponent::horizontalKeyboard && wheel.deltaX != 0)
                  ? wheel.deltaX : (keyboardComponent->getOrientation() == MidiKeyboardComponent::verticalKeyboardFacingLeft ? wheel.deltaY
                                                                                                                         : -wheel.deltaY);

    keyboardComponent->setLowestVisibleKeyFloat(keyboardComponent->getFirstKey() - amount * keyboardComponent->getKeyWidth());

    gridComponent->updateNoteLineRanges(keyboardComponent->getKeyStartPosition(0));
}

void PianoRollComponent::paint(Graphics &g) {
    Component::paint(g);
}

void PianoRollComponent::timerCallback() {
    repaint();
}

void PianoRollComponent::resized()
{
    auto r = getBounds();
    keyboardComponent->setBounds(r.removeFromLeft(50));
    gridComponent->setBounds(r);
    gridComponent->updateNoteLineRanges(keyboardComponent->getKeyStartPosition(0));
}
