//
// Created by Andrei Gheorghe on 07.08.2022.
//
#pragma once

#include "JuceHeader.h"
#include "NoteRectangle.h"

class GridComponent: public Component {
private:
    OwnedArray<Range<int>> noteLineRanges;
    OwnedArray<Range<int>> noteRowRanges;
    std::list<NoteRectangle> notes;
    NoteRectangle pressed, new_position;
    NoteRectangle find_note_rect(Point<int> position);
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
