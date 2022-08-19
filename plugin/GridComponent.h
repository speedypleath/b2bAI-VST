//
// Created by Andrei Gheorghe on 07.08.2022.
//
#pragma once

#include "JuceHeader.h"

class GridComponent: public Component {
private:
    OwnedArray<Range<int>> noteLineRanges;
    OwnedArray<Range<int>> noteRowRanges;
    std::list<Rectangle<int>> notes;
    Rectangle<int> pressed, new_position;
    Rectangle<int> find_note_rect(Point<int> position);
public:
    GridComponent();

    void updateNoteLineRanges(int firstKeyStartPosition);

    void paint(Graphics &g) override;
    void mouseMove(const MouseEvent &event) override;
    void mouseDown(const MouseEvent &event) override;
    void mouseDrag(const MouseEvent &event) override;
    void mouseDoubleClick(const MouseEvent &event) override;
    void mouseUp(const MouseEvent &event) override;
};
