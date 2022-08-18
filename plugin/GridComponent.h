//
// Created by Andrei Gheorghe on 07.08.2022.
//
#pragma once

#include "JuceHeader.h"

class GridComponent: public Component {
private:
    OwnedArray<Range<float>> noteLineRanges;
    OwnedArray<Range<float>> noteRowRanges;
public:
    GridComponent();

    void updateNoteLineRanges(float firstKeyStartPosition);

    void paint(Graphics &g) override;
};
