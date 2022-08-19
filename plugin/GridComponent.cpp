//
// Created by Andrei Gheorghe on 07.08.2022.
//

#include "GridComponent.h"
#include "Constants.h"
#include <iostream>
#include <algorithm>

GridComponent::GridComponent()
{
    for (int i = 0; i <= 127; i++)
        noteLineRanges.add(new Range<int>(0, constants::noteLineWidth));

    for (int i = 0; i <= 32; i++)
        noteRowRanges.add(new Range<int>(0, 0));
}

void GridComponent::updateNoteLineRanges(int firstKeyStartPosition)
{
    for (int i = 0; i <= 127; i++)
    {
        noteLineRanges[i]->setStart(getHeight() - firstKeyStartPosition - (i + 1) * constants::noteLineWidth);
        noteLineRanges[i]->setLength(constants::noteLineWidth);
    }

    for (int i = 0; i <= 32; i++) {
        noteRowRanges[i]->setStart(getWidth() / 32 * i);
        noteRowRanges[i]->setLength(getWidth() / 32);
    }
    repaint();
}

void GridComponent::paint(Graphics& g)
{
    int height = getHeight();
    int width = getWidth();
    int noteLen = width / 32;
    int barLen = noteLen * 4;

    // Draw the background
    for (int i = 1; i < 8; i += 2) {
        auto shadow = DropShadow(Colours::black, 1, Point<int>(0, 0));
        auto rect = Rectangle<int>(i * barLen, 0, barLen, height);
        shadow.drawForRectangle(g, rect);
    }
    g.setColour(Colours::white);

    // Draw note lines
    for (int i = 0; i < noteLineRanges.size(); i++) {
        auto rect = Rectangle<int>(0, noteLineRanges[i]->getStart(), width, noteLineRanges[i]->getLength());
        if (rect.getBottom() < 0 || rect.getY() >= height)
            continue;
        g.drawRect(rect, 1);
        if (!MidiMessage::isMidiNoteBlack(i))
            continue;
        g.fillRect(rect);
    }

    g.setColour(Colours::lightgrey);
    // Draw beats lines
    for (auto noteRowRange : noteRowRanges) {
        g.fillRect(static_cast<int>(noteRowRange->getStart()), 0, 1, height);
    }

    g.setColour(Colours::green);
    // draw notes
    for (auto note : notes) {
        g.fillRect(*note);
    }
}

void GridComponent::mouseDown(const MouseEvent &event) {
    if(event.mouseWasClicked()) {
        Point<int> position = event.getPosition();
        Range<int> *x = nullptr, *y = nullptr;
        for (auto range: noteRowRanges)
            if (range->contains(position.getX()))
                x = range;

        for (auto range: noteLineRanges)
            if (range->contains(position.getY()))
                y = range;

        if (x == nullptr || y == nullptr)
            return;

        auto pressed = new Rectangle<int>(x->getStart(), y->getStart(), x->getLength(), y->getLength());
        auto found = std::find_if(notes.begin(), notes.end(), [pressed](Rectangle<int> *a) {
            return a->getX() == pressed->getX() && a->getY() == pressed->getY();
        });
        if(event.mods.isLeftButtonDown()) {
            if (found == notes.end())
                notes.add(pressed);
            else {
                std::cout << "already pressed";
                delete pressed;
            }
        }
        else{
            if(found != notes.end())
            notes.removeObject(*found);
        }
    }
}

