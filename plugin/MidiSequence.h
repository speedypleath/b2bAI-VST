//
// Created by Andrei Gheorghe on 21.08.2022.
//

#pragma once
#include "NoteRectangle.h"
#include "JuceHeader.h"

class MidiSequence: public Array<NoteRectangle> {
    double endTime;
public:
    void load(const File& file);

    double getEndTime() const;

    void save(const File& file);
};
