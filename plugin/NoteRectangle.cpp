//
// Created by Andrei Gheorghe on 18.08.2022.
//

#include "NoteRectangle.h"

int NoteRectangle::getPitch() const {
    return pitch;
}

void NoteRectangle::setPitch(int p) {
    pitch = p;
}

int NoteRectangle::getVelocity() const {
    return velocity;
}

bool NoteRectangle::operator==(const NoteRectangle &rhs) const {
    return start == rhs.start &&
           end == rhs.end &&
           pitch == rhs.pitch &&
           velocity == rhs.velocity;
}

bool NoteRectangle::operator!=(const NoteRectangle &rhs) const {
    return !(rhs == *this);
}

void NoteRectangle::setVelocity(int v) {
    velocity = v;
}

NoteRectangle::NoteRectangle(int x, int y, int width, int height, int i) : Rectangle<int>(x, y, width, height), index(i) { }
NoteRectangle::NoteRectangle(int p, int v, double s, double e) : start(s), end(e), pitch(p), velocity(v) { }

std::ostream &operator<<(std::ostream &os, const NoteRectangle &note) {
    os << "start: " << note.getX() << " end: " << note.getRight() << " pitch: " << note.pitch << " velocity: " << note.velocity << " y: " << note.getY();
    return os;
}

double NoteRectangle::getStart() const {
    return start;
}

void NoteRectangle::setStart(double s) {
    start = s;
}

double NoteRectangle::getEnd() const {
    return end;
}

void NoteRectangle::setEnd(double e) {
    end = e;
}

int NoteRectangle::getIndex() const {
    return index;
}

void NoteRectangle::setIndex(int i) {
    index = i;
}