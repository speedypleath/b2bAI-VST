//
// Created by Andrei Gheorghe on 18.08.2022.
//
#pragma once

#include <ostream>
#include "JuceHeader.h"
#include "midi_generator/note.h"

class NoteRectangle: public Rectangle<int> {
public:
    NoteRectangle(int x=0, int y=0, int width=0, int height=0, int p=0);
    NoteRectangle(int p, int v, double s, double e);
    int getPitch() const;

    void setPitch(int pitch);

    friend std::ostream &operator<<(std::ostream &os, const NoteRectangle &note);

    int getVelocity() const;

    void setVelocity(int velocity);

    bool operator==(const NoteRectangle &rhs) const;

    bool operator!=(const NoteRectangle &rhs) const;

    double getStart() const;

    void setStart(double start);

    double getEnd() const;

    void setEnd(double end);
private:
    midi_generator::Note note;
};