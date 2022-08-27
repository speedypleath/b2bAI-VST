//
// Created by Andrei Gheorghe on 21.08.2022.
//

#include "MidiSequence.h"
#include <iostream>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>


namespace logging = boost::log;

void MidiSequence::load(const File& file) {
    FileInputStream stream(file);

    if(file.existsAsFile()) {
        jassert(stream.getStatus().wasOk());

        MidiFile midiFile;
        jassert(midiFile.readFrom(stream, true));

        auto numTracks = midiFile.getNumTracks();
        auto tracks = new MidiMessageSequence[static_cast<unsigned long>(numTracks)];

        for(int i = 0; i < numTracks; i++)
            tracks[i] = *midiFile.getTrack(i);

        MidiMessageSequence sequence;

        tracks[0].extractMidiChannelMessages(1, sequence, false);
        resize(sequence.getNumEvents());
        sequence.updateMatchedPairs();
        endTime = sequence.getEndTime();

        std::transform(sequence.begin(), sequence.end(), begin(), [] (MidiMessageSequence::MidiEventHolder *event) {
            return event->message.isNoteOn() ? NoteRectangle(
                        event->message.getNoteNumber(),
                        event->message.getVelocity(),
                        event->message.getTimeStamp(),
                        event->noteOffObject->message.getTimeStamp()) : NoteRectangle{};
        });

        removeIf([](NoteRectangle note) { return note.getPitch() == 0; });

        delete[] tracks;
    }
}

void MidiSequence::save(const File&) {
    // Save midi in python
}

double MidiSequence::getEndTime() const {
    return endTime;
}
