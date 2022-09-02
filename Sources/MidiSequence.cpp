//
// Created by Andrei Gheorghe on 21.08.2022.
//

#include "MidiSequence.h"
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <midi_generator/API.h>
#include <midi_generator/note.h>
#include <list>


MidiSequence::~MidiSequence() {
    pybind11::finalize_interpreter();
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

        removeIf([](const NoteRectangle& note) { return note.getPitch() == 0; });

        delete[] tracks;
    }
}

void MidiSequence::generate() {
    if(!initialised) {
        pybind11::initialize_interpreter();
        initialised = true;
    }
    std::list<midi_generator::Note> notes = midi_generator::generate();

    endTime = notes.back().end;

    std::transform(notes.begin(), notes.end(), begin(), [] (midi_generator::Note note) {
        BOOST_LOG_TRIVIAL(info) << note;
        return NoteRectangle(note.pitch, note.velocity, note.start, note.end);
    });
}

void MidiSequence::save(const File&) {
    // Py_Initialize();
    // object main_module = import("midi_generator.utils.output");
    // object main_namespace = main_module.attr("__dict__");
    // object save_midi = main_namespace["write_file"];
    // try {
    //     std::list<NoteRectangle> notes;
    //     std::copy(begin(), end(), notes);
    //     class_<std::list<int> >("list_int")
    // .def("assign", &list_assign<int>)
    // // ...
    // ;
    // } catch (error_already_set) {
    //     PyErr_Print();

    // }

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
    endTime = notes.back().end;

    std::transform(notes.begin(), notes.end(), begin(), [] (midi_generator::Note note) {
        BOOST_LOG_TRIVIAL(info) << note;
        return NoteRectangle(note.pitch, note.velocity, note.start, note.end);
    });
}


