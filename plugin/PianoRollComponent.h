//
// Created by Andrei Gheorghe on 07.08.2022.
//
#pragma once

#include "JuceHeader.h"
#include "KeyboardComponent.h"
#include "GridComponent.h"

class PianoRollComponent: public Component {
private:
    std::unique_ptr<KeyboardComponent> keyboardComponent;
    std::unique_ptr<GridComponent> gridComponent;

public:
    PianoRollComponent();
};

