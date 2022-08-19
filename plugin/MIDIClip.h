//
// Created by Andrei Gheorghe on 18.08.2022.
//
#pragma once

#include <ostream>
#include "JuceHeader.h"

class Note: public Rectangle<int> {
public:
    Note(int x=0, int y=0, int width=0, int height=0, int pitch=0, int velocity=100);

    [[nodiscard]] int getPitch() const;

    void setPitch(int pitch);

    friend std::ostream &operator<<(std::ostream &os, const Note &note);

    [[nodiscard]] int getVelocity() const;

    void setVelocity(int velocity);

    bool operator==(const Note &rhs) const;

    bool operator!=(const Note &rhs) const;

private:
    int pitch{};
    int velocity{};
public:

};

class MIDIClip {

};