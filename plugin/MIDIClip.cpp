//
// Created by Andrei Gheorghe on 18.08.2022.
//

#include "MIDIClip.h"

int Note::getPitch() const {
    return pitch;
}

void Note::setPitch(int pitch) {
    Note::pitch = pitch;
}

int Note::getVelocity() const {
    return velocity;
}

bool Note::operator==(const Note &rhs) const {
    return static_cast<const Rectangle<int> &>(*this) == static_cast<const Rectangle<int> &>(rhs) &&
           pitch == rhs.pitch &&
           velocity == rhs.velocity;
}

bool Note::operator!=(const Note &rhs) const {
    return !(rhs == *this);
}

void Note::setVelocity(int velocity) {
    Note::velocity = velocity;
}

Note::Note(int x, int y, int width, int height, int pitch, int velocity) : Rectangle<int>(x, y, width, height), pitch(pitch), velocity(velocity) { }

std::ostream &operator<<(std::ostream &os, const Note &note) {
    os << "start: " << note.getX() << " end: " << note.getRight() << " pitch: " << note.pitch << " velocity: " << note.velocity << " y: " << note.getY();
    return os;
}
