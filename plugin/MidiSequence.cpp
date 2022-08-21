//
// Created by Andrei Gheorghe on 21.08.2022.
//

#include "MidiSequence.h"
#include <iostream>

template <typename Cont, typename Pred>
Cont filter(const Cont &container, Pred predicate) {
    Cont result;
    std::copy_if(container.begin(), container.end(), std::back_inserter(result), predicate);
    return result;
}

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

void MidiSequence::save(const File& file) {

}

double MidiSequence::getEndTime() const {
    return endTime;
}
