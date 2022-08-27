//
// Created by Andrei Gheorghe on 18.08.2022.
//

#include "NoteRectangle.h"

int NoteRectangle::getPitch() const {
    return note.pitch;
}

void NoteRectangle::setPitch(int p) {
    note.pitch = p;
}

int NoteRectangle::getVelocity() const {
    return note.velocity;
}

bool NoteRectangle::operator==(const NoteRectangle &rhs) const {
    return note == rhs.note;
}

bool NoteRectangle::operator!=(const NoteRectangle &rhs) const {
    return !(rhs == *this);
}

void NoteRectangle::setVelocity(int v) {
    note.velocity = v;
}

NoteRectangle::NoteRectangle(int x, int y, int width, int height, int p) : Rectangle<int>(x, y, width, height), note(p, 100, x, y) { }
NoteRectangle::NoteRectangle(int p, int v, double s, double e) : note(p, v, s, e) { }



std::ostream &operator<<(std::ostream &os, const NoteRectangle &note) {
    os << "start: " << note.getX() << " end: " << note.getRight() << " pitch: "
       << note.getPitch() << " velocity: " << note.getVelocity() << " width: "
       << note.getWidth() << " height: " << note.getHeight();

    return os;
}

double NoteRectangle::getStart() const {
    return note.start;
}

void NoteRectangle::setStart(double s) {
    note.start = s;
}

double NoteRectangle::getEnd() const {
    return note.end;
}

void NoteRectangle::setEnd(double e) {
    note.end = e;
}