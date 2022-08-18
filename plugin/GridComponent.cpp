//
// Created by Andrei Gheorghe on 07.08.2022.
//

#include "GridComponent.h"
#include "Constants.h"
#include <iostream>

GridComponent::GridComponent()
{
    for (int i = 0; i <= 127; i++)
        noteLineRanges.add(new Range<float>(0, constants::noteLineWidth));

    for (int i = 0; i <= 32; i++)
        noteRowRanges.add(new Range<float>(0, 0));
}

void GridComponent::updateNoteLineRanges(float firstKeyStartPosition)
{
    for (int i = 0; i <= 127; i++)
    {
        noteLineRanges[i]->setStart(static_cast<float>(getHeight()) - firstKeyStartPosition - (static_cast<float>(i + 1)) * constants::noteLineWidth);
        noteLineRanges[i]->setLength(constants::noteLineWidth);
    }

    for (int i = 0; i <= 32; i++) {
        noteRowRanges[i]->setStart(floor(static_cast<float>(getWidth()) / 32) * (static_cast<float>(i)));
        noteRowRanges[i]->setLength(floor(static_cast<float>(getWidth()) / 32));
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
        auto rect = Rectangle<float>(0, noteLineRanges[i]->getStart(), static_cast<float>(width), noteLineRanges[i]->getLength());
        if (rect.getBottom() < 0 || rect.getY() >= static_cast<float>(height)) // skip if the line is out of bounds
            continue;
        g.drawRect(rect, 0.5f);
        if (!MidiMessage::isMidiNoteBlack(i))
            continue;
        g.fillRect(rect);
    }

    g.setColour(Colours::lightgrey);
    // Draw beats lines
    for (auto noteRowRange : noteRowRanges) {
        g.fillRect(static_cast<int>(noteRowRange->getStart()), 0, 1, height);
    }
}