//
// Created by Andrei Gheorghe on 07.08.2022.
//

#include "GridComponent.h"
#include "Constants.h"
#include <algorithm>
#include <boost/log/trivial.hpp>

namespace logging = boost::log;

GridComponent::GridComponent()
{
    for (int i = 0; i <= 127; i++)
        noteLineRanges.add(new Range<int>(0, constants::noteLineWidth));

    for (int i = 0; i <= 32; i++)
        noteRowRanges.add(new Range<int>(0, 0));
}

int GridComponent::normalise(double v, double vMax) {
    double aux = v / vMax;
    aux *= getWidth();
    return static_cast<int>(aux);
}

void GridComponent::updateNoteLineRanges(int firstKeyStartPosition)
{
    BOOST_LOG_TRIVIAL(debug) << "update grid\n";
    for (int i = 0; i <= 127; i++)
    {
        noteLineRanges[i]->setStart(getHeight() - firstKeyStartPosition - (i + 1) * constants::noteLineWidth);
        noteLineRanges[i]->setLength(constants::noteLineWidth);
    }

    for (int i = 0; i <= 31; i++) {
        noteRowRanges[i]->setStart(getWidth() * i / 32);
        noteRowRanges[i]->setLength(getWidth() / 32);
    }

    if(notes)
        for (auto &note: *notes) {
            note.setBottom(noteLineRanges[note.getPitch()]->getStart());
            note.setY(noteLineRanges[note.getPitch()]->getEnd());
            note.setHeight(noteLineRanges[note.getPitch()]->getLength());

            int start = normalise(note.getStart(), notes->getEndTime());
            int end = normalise(note.getEnd(), notes->getEndTime());
            BOOST_LOG_TRIVIAL(debug) << "duration: " << start << " -> " << end;


            Range<int> startNote;
            for(auto rect: noteRowRanges) {
                if(rect->getStart() > start - getWidth() / 64) {
                    startNote = *rect;
                    break;
                }
            }

            BOOST_LOG_TRIVIAL(debug) << "X-axis: " << startNote.getStart() << " -> " << startNote.getEnd();

            Range<int> endNote;
            for(auto rect: noteRowRanges) {
                if(rect->getEnd() > end + getWidth() / 64 - getWidth() / 32) {
                    endNote = *rect;
                    break;
                }
            }

            BOOST_LOG_TRIVIAL(debug) << "Y-axis: " << endNote.getStart() << " -> " << endNote.getEnd();

            note.setX(startNote.getStart());
            note.setRight(endNote.getEnd());
            note.setWidth(endNote.getEnd() - startNote.getStart());

            BOOST_LOG_TRIVIAL(debug) << note;
        }
    repaint();
}

void GridComponent::paint(Graphics& g)
{
    int height = getHeight();
    int width = getWidth();
    // Draw the background
    for (int i = 0; i < 8; i += 2) {
        auto shadow = DropShadow(Colours::black, 1, Point<int>(0, 0));
        auto rect = Rectangle<int>(noteRowRanges[i * 4]->getStart(),
                                   0,
                                   noteRowRanges[(i + 1) * 4]->getStart() - noteRowRanges[i * 4]->getStart(),
                                   height);
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
        g.fillRect(static_cast<int>(noteRowRange->getStart() - 1), 0, 1, height);
    }

    // draw notes
    g.setColour(Colours::green);
    if(notes)
        for (const auto& note : *notes) {
            if(pressed == note) {
                g.fillRect(new_position.expanded(1));
                continue;
            }

            g.fillRect(note);
        }
}

void GridComponent::mouseMove(const MouseEvent &event) {
    auto cursor = find_note_rect(event.getPosition());
    if(!notes)
        return;
    auto it = std::find_if(notes->begin(), notes->end(), [cursor](auto a) {
        return !cursor.getIntersection(a).isEmpty();
    });
    int x = event.getPosition().getX();

    if(it == notes->end()) {
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
    if(!notes)
        return;
    pressed = find_note_rect(event.getPosition());

    if(getMouseCursor() == MouseCursor::PointingHandCursor)
        setMouseCursor(MouseCursor::DraggingHandCursor);

    auto found = std::find_if(notes->begin(), notes->end(), [this](auto a) {
        return !pressed.getIntersection(a).isEmpty();
    });

    if(found == notes->end())
        return;

    if(event.mods.isRightButtonDown()) {
        notes->remove(found);
        return;
    }

    pressed = *&*found;

    new_position = pressed;
}

void GridComponent::mouseDrag(const juce::MouseEvent &event) {
    if(!notes)
        return;
    if(event.mouseWasDraggedSinceMouseDown()) {
        if(getMouseCursor() == MouseCursor::DraggingHandCursor) {
            new_position.setX(static_cast<int>(event.position.getX()) - pressed.getWidth() / 2);
            new_position.setY(static_cast<int>(event.position.getY()) - pressed.getHeight() / 2);
        }

        if(getMouseCursor() == MouseCursor::RightEdgeResizeCursor)
            new_position.setRight(static_cast<int>(event.position.getX()));

        if(getMouseCursor() == MouseCursor::LeftEdgeResizeCursor)
            new_position.setLeft(static_cast<int>(event.position.getX()));

    }
}

void GridComponent::mouseDoubleClick(const MouseEvent &event) {
    if(!notes)
        return;
    auto it = std::find(notes->begin(), notes->end(), pressed);
    if (it == notes->end())
        notes->add(pressed);
    else {
        BOOST_LOG_TRIVIAL(info) << "already pressed";
    }
    pressed = {};
    new_position = {};
}

void GridComponent::mouseUp(const MouseEvent &event) {
    if(!notes)
        return;
    if(getMouseCursor() == MouseCursor::NormalCursor && event.getNumberOfClicks() > 1)
        return;

    if(getMouseCursor() == MouseCursor::NormalCursor && event.getNumberOfClicks() == 1) {
        pressed = {};
        new_position = {};
        return;
    }

    NoteRectangle new_note;
    if(getMouseCursor() == MouseCursor::LeftEdgeResizeCursor || getMouseCursor() == MouseCursor::RightEdgeResizeCursor)
        new_note = new_position;

    if(getMouseCursor() == MouseCursor::DraggingHandCursor && event.mods.isLeftButtonDown()) {
       new_note = find_note_rect(Point<int>(new_position.getTopLeft()));
       new_note.setX(new_position.getX());
       new_note.setWidth(new_position.getWidth());
    }

    notes->removeIf([this](auto other) { return pressed == other; } );
    notes->add(new_note);
}

NoteRectangle GridComponent::find_note_rect(Point<int> position) {
    Range<int> *x = nullptr, *y = nullptr;
    for (auto range: noteRowRanges)
        if (range->contains(position.getX()))
            x = range;

    int index = 0;
    for (int i = 0; i <= 127; i++)
        if(noteLineRanges[i]->contains(position.getY())) {
            y = noteLineRanges[i];
            index = i;
    }

    if (x == nullptr || y == nullptr)
        return { };


    return {x->getStart(), y->getStart(), x->getLength(), y->getLength(), index};
}

void GridComponent::setMidiSequence(MidiSequence *sequence) {
    notes = sequence;
}
