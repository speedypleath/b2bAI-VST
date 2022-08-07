//
// Created by Andrei Gheorghe on 07.08.2022.
//

#include "GridComponent.h"
#include "Constants.h"

GridComponent::GridComponent()
{

    for (int i = 0; i <= 127; ++i)
        noteLineRanges.add(new Range<float>(0, constants::noteLineWidth));
}

void GridComponent::updateNoteLineRanges(float firstKeyStartPosition)
{
    for (int i = 0; i <= 127; ++i)
    {
        noteLineRanges[i]->setStart(static_cast<float>(getHeight()) - firstKeyStartPosition - (static_cast<float>(i +1)) * constants::noteLineWidth);
        noteLineRanges[i]->setLength(constants::noteLineWidth);
    }
    repaint();
}

void GridComponent::paint(Graphics& g)
{
    // Draw the background
    g.fillAll(Colours::lightgrey);

    // Draw note lines
    g.setColour(Colours::grey);
    for (int i = 0; i < noteLineRanges.size(); ++i) {
        auto rect = Rectangle<float>(0, noteLineRanges[i]->getStart(), static_cast<float>(getWidth()), noteLineRanges[i]->getLength());
        if (rect.getBottom() < 0 || rect.getY() >= static_cast<float>(getHeight())) // skip if the line is out of bounds
            continue;
        g.drawRect(rect, 0.5f);
        if (!MidiMessage::isMidiNoteBlack(i))
            continue;
        g.fillRect(rect);
    }

    int width = getWidth();

    for (int i = 0; i < width; i += width / 32) {
        g.setColour(Colours::white);
        g.fillRect(i, 0, 1, getHeight());
    }
}