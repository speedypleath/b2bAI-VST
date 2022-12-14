//
// Created by Andrei Gheorghe on 21.08.2022.
//

#pragma once
#include "NoteRectangle.h"
#include "JuceHeader.h"
#include <pybind11/embed.h>

class MidiSequence: public Array<NoteRectangle> {
    double endTime = 16.0f;
public:
//    MidiSequence();
    void load(const File& file);
    void load_notes(std::list<midi_generator::Note> notes);

    std::list<midi_generator::Note> to_notes();

    double getEndTime() const;

    double normalise(double, double) const;
};
