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
    return static_cast<const Rectangle<int> &>(*this) == static_cast<const Rectangle<int> &>(rhs) &&
           pitch == rhs.pitch &&
           velocity == rhs.velocity;
}

bool NoteRectangle::operator!=(const NoteRectangle &rhs) const {
    return !(rhs == *this);
}

void NoteRectangle::setVelocity(int v) {
    velocity = v;
}

NoteRectangle::NoteRectangle(int x, int y, int width, int height, int p, int v) : Rectangle<int>(x, y, width, height), pitch(p), velocity(v) { }

std::ostream &operator<<(std::ostream &os, const NoteRectangle &note) {
    os << "start: " << note.getX() << " end: " << note.getRight() << " pitch: " << note.pitch << " velocity: " << note.velocity << " y: " << note.getY();
    return os;
}
