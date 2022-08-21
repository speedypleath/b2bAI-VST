//
// Created by Andrei Gheorghe on 21.08.2022.
//
#pragma once

struct Note {
    int pitch;
    int velocity;
    double start;
    double end;

    Note(int p, int v, double s, double e) : pitch(p), velocity(v), start(s), end(e) {}

    bool operator==(const Note &rhs) const {
        return pitch == rhs.pitch &&
               velocity == rhs.velocity &&
               start == rhs.start &&
               end == rhs.end;
    }

    bool operator!=(const Note &rhs) const {
        return !(rhs == *this);
    }
};
