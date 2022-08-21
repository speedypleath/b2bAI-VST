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
        std::cout << stream.getStatus().getErrorMessage() << std::endl;
        jassert(stream.getStatus().wasOk());

        MidiFile midiFile;
        jassert(midiFile.readFrom(stream, true));

        auto numTracks = midiFile.getNumTracks();
        std::cout << "Number of tracks: " << numTracks << std::endl;
        auto tracks = new MidiMessageSequence[static_cast<unsigned long>(numTracks)];

        for(int i = 0; i < numTracks; i++)
            tracks[i] = *midiFile.getTrack(i);

        MidiMessageSequence sequence;

        tracks[0].extractMidiChannelMessages(1, sequence, false);
        resize(sequence.getNumEvents());
        sequence.updateMatchedPairs();

        std::transform(sequence.begin(), sequence.end(), begin(), [] (MidiMessageSequence::MidiEventHolder *event) {
            std::cout << event->message.getDescription() << std::endl;
            return event->message.isNoteOn() ? NoteRectangle(
                        event->message.getNoteNumber(),
                        event->message.getVelocity(),
                        event->message.getTimeStamp(),
                        event->noteOffObject->message.getTimeStamp()
                    ) : NoteRectangle{};
        });

        removeIf([](NoteRectangle note) { return note.getPitch() == 0; });

        std::for_each(begin(), end(), [](NoteRectangle note ) { std::cout << note << std::endl; } );

        delete[] tracks;
    }
}

void MidiSequence::save(const File& file) {

}