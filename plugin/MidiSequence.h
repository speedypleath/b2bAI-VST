//
// Created by Andrei Gheorghe on 21.08.2022.
//

#pragma once
#include "NoteRectangle.h"
#include "JuceHeader.h"

class MidiSequence: public Array<NoteRectangle> {
public:
    void load(const File& file);
    void save(const File& file);
};
