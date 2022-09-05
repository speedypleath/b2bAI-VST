//
// Created by Andrei Gheorghe on 21.08.2022.
//

#include "MidiSequence.h"
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <note.h>
#include <list>


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
        endTime = 16.0f;

        auto aux = sequence.getEndTime();

        std::transform(sequence.begin(), sequence.end(), begin(), [this, aux] (MidiMessageSequence::MidiEventHolder *event) {
            return event->message.isNoteOn() ? NoteRectangle(
                        event->message.getNoteNumber(),
                        event->message.getVelocity(),
                        normalise(event->message.getTimeStamp(), aux),
                        normalise(event->noteOffObject->message.getTimeStamp(), aux)) : NoteRectangle{};
        });

        removeIf([](const NoteRectangle& note) { return note.getPitch() == 0; });

        delete[] tracks;
    }
}

double MidiSequence::normalise(double v, double vMax) const {
    double aux = v / vMax * endTime;
    return aux;
}

double MidiSequence::getEndTime() const {
    return endTime;
}

std::list<midi_generator::Note> MidiSequence::to_notes() {
    std::list<midi_generator::Note> notes;
    std::for_each(begin(), end(), [&notes](const NoteRectangle& rect) {
        notes.emplace_back(midi_generator::Note(rect.getPitch(), rect.getVelocity(), rect.getStart(), rect.getEnd()));
    });
    return notes;
}

void MidiSequence::load_notes(std::list<midi_generator::Note> notes) {
    clear();
    std::for_each(notes.begin(), notes.end(), [this] (const midi_generator::Note& note){
        BOOST_LOG_TRIVIAL(info) << note;
        this->add(NoteRectangle(note.pitch, note.velocity, note.start, note.end));
    });
}


