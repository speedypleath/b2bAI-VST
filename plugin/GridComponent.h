//
// Created by Andrei Gheorghe on 07.08.2022.
//
#pragma once

#include "JuceHeader.h"

class GridComponent: public Component {
private:
    OwnedArray<Range<int>> noteLineRanges;
    OwnedArray<Range<int>> noteRowRanges;
    OwnedArray<Rectangle<int>> notes;
public:
    GridComponent();

    void updateNoteLineRanges(int firstKeyStartPosition);

    void paint(Graphics &g) override;
    void mouseDown(const MouseEvent &event) override;
};
