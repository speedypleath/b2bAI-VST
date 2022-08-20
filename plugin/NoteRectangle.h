//
// Created by Andrei Gheorghe on 18.08.2022.
//
#pragma once

#include <ostream>
#include "JuceHeader.h"

class NoteRectangle: public Rectangle<int> {
public:
    NoteRectangle(int x=0, int y=0, int width=0, int height=0, int pitch=0, int velocity=100);

    [[nodiscard]] int getPitch() const;

    void setPitch(int pitch);

    friend std::ostream &operator<<(std::ostream &os, const NoteRectangle &note);

    [[nodiscard]] int getVelocity() const;

    void setVelocity(int velocity);

    bool operator==(const NoteRectangle &rhs) const;

    bool operator!=(const NoteRectangle &rhs) const;

private:
    int pitch{};
    int velocity{};
public:

};