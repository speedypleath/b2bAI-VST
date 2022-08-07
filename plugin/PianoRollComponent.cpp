//
// Created by Andrei Gheorghe on 07.08.2022.
//

#include "PianoRollComponent.h"

PianoRollComponent::PianoRollComponent() {
    keyboardComponent = std::make_unique<KeyboardComponent>();
    gridComponent = std::make_unique<GridComponent>();
    addAndMakeVisible(*keyboardComponent);
    addAndMakeVisible(*gridComponent);
    setSize(600, 400);
}
