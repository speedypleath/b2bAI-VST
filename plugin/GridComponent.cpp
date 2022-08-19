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

    // draw notes
    g.setColour(Colours::green);
    for (auto note : notes) {
        if(pressed == note && getMouseCursor() == MouseCursor::DraggingHandCursor) {
            g.fillRect(new_position.getX() - pressed.getWidth() / 2,
                       new_position.getY() - pressed.getHeight() / 2,
                       pressed.getWidth() + pressed.getWidth() / 8,
                       pressed.getHeight() + pressed.getHeight() / 8);
            continue;
        }

        g.fillRect(note);
    }
}

void GridComponent::mouseMove(const MouseEvent &event) {
    auto cursor = find_note_rect(event.getPosition());
    auto it = std::find(notes.begin(), notes.end(), cursor);
    int x = event.getPosition().getX();

    if(it == notes.end()) {
        setMouseCursor(MouseCursor::NormalCursor);
        return;
    }

    if(x < it->getX() + 5) {
        setMouseCursor(MouseCursor::LeftEdgeResizeCursor);
        return;
    }

    if(x > it->getRight() - 5) {
        setMouseCursor(MouseCursor::RightEdgeResizeCursor);
        return;
    }

    setMouseCursor(MouseCursor::PointingHandCursor);
}

void GridComponent::mouseDown(const MouseEvent &event) {
    pressed = find_note_rect(event.getPosition());
    if(getMouseCursor() == MouseCursor::PointingHandCursor) {
        new_position = event.getPosition();
        setMouseCursor(MouseCursor::DraggingHandCursor);
    }
    auto found = std::find_if(notes.begin(), notes.end(), [this](auto a) {
        return pressed.intersectRectangle(a);
    });

    if(event.mouseWasClicked() && event.mods.isRightButtonDown() && found != notes.end()) {
        notes.erase(found);
    }
}

void GridComponent::mouseDrag(const juce::MouseEvent &event) {
    Rectangle<int> *it = &*std::find(notes.begin(), notes.end(), pressed);
    if(event.mouseWasDraggedSinceMouseDown()) {
        if(getMouseCursor() == MouseCursor::PointingHandCursor || getMouseCursor() == MouseCursor::DraggingHandCursor)
            new_position = event.getPosition();

        std::cout << "drag x" << event.getPosition().getX() << std::endl;

        if(getMouseCursor() == MouseCursor::RightEdgeResizeCursor) {
            it->setRight(event.getPosition().getX());
            pressed = *it;
            std::cout << "set right" <<std::endl;
        }

        if(getMouseCursor() == MouseCursor::LeftEdgeResizeCursor) {
            it->setLeft(event.getPosition().getX());
            pressed = *it;
            std::cout << "set left" <<std::endl;
        }
    }
}

void GridComponent::mouseDoubleClick(const MouseEvent &event) {
    auto it = std::find(notes.begin(), notes.end(), pressed);
    if (it == notes.end())
        notes.push_back(pressed);
    else {
        std::cout << "already pressed";
    }
    pressed = {};
    new_position = {};
}

void GridComponent::mouseUp(const MouseEvent &event) {

    if(event.mouseWasDraggedSinceMouseDown()) {
        auto new_note = find_note_rect(new_position);
        if(pressed != new_note && getMouseCursor() == MouseCursor::DraggingHandCursor) {
            notes.remove(pressed);
            notes.push_back(new_note);
            pressed = {};
            new_position = {};
        }
    }

    if(event.getNumberOfClicks() == 1) {
        pressed = {};
        new_position = {};
    }
}

Rectangle<int> GridComponent::find_note_rect(Point<int> position) {
    Range<int> *x = nullptr, *y = nullptr;
    for (auto range: noteRowRanges)
        if (range->contains(position.getX()))
            x = range;

    for (auto range: noteLineRanges)
        if (range->contains(position.getY()))
            y = range;

    if (x == nullptr || y == nullptr)
        return { };


    return {x->getStart(), y->getStart(), x->getLength(), y->getLength()};
}
