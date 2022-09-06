#include "API.h"

#include <utility>
#include "note.h"
#include "pybind11/pybind11.h"

namespace py = pybind11;
using namespace py::literals;

template<typename T>
py::list to_python_list(std::list<T> std_list) {
    typename std::list<T>::iterator iter;
    py::list python_list;
    for (iter = std_list.begin(); iter != std_list.end(); ++iter) {
        python_list.append(*iter);
    }
    return python_list;
}

std::list<midi_generator::Note> midi_generator::generate(const midi_generator::Configuration& config) {
    try {
        py::module_ commands = py::module_::import("midi_generator.commands");
        py::object result = commands.attr("generate")();
        std::list<midi_generator::Note> notes;
        for(const auto& obj: result)
            notes.emplace_back(
                    obj.attr("pitch").cast<int>(),
                    obj.attr("velocity").cast<int>(),
                    obj.attr("start").cast<double>(),
                    obj.attr("end").cast<double>()
            );

        return notes;
    } catch (py::error_already_set &error) {
        error.discard_as_unraisable(__func__ );
        return {};
    }
}
std::list<midi_generator::Note> midi_generator::generate() {
    return midi_generator::generate(Configuration{});
}

std::list<midi_generator::Note> midi_generator::mutate(std::list<midi_generator::Note> notes) {
    try {
        py::module_ commands = py::module_::import("midi_generator.commands");
        py::object result = commands.attr("mutate")(to_python_list(std::move(notes)));

        std::list<midi_generator::Note> mutant;
        for(const auto& obj: result)
            mutant.emplace_back(
                    obj.attr("pitch").cast<int>(),
                    obj.attr("velocity").cast<int>(),
                    obj.attr("start").cast<double>(),
                    obj.attr("end").cast<double>()
            );
        return mutant;
    } catch (py::error_already_set &error) {
        error.discard_as_unraisable(__func__ );
        return {};
    }
}

std::list<midi_generator::Note> midi_generator::continue_sequence(std::list<midi_generator::Note> notes) {
    try {
        py::module_ commands = py::module_::import("midi_generator.commands");
        py::object result = commands.attr("continue_sequence")(to_python_list(std::move(notes)));

        std::list<midi_generator::Note> continued;
        for(const auto& obj: result)
            continued.emplace_back(
                    obj.attr("pitch").cast<int>(),
                    obj.attr("velocity").cast<int>(),
                    obj.attr("start").cast<double>(),
                    obj.attr("end").cast<double>()
            );
        return continued;
    } catch (py::error_already_set &error) {
        error.discard_as_unraisable(__func__ );
        std::cout << error.what();
        return {};
    }
}

std::list<midi_generator::Note> midi_generator::combine(const std::list<std::list<midi_generator::Note>>& sequences) {
    try {
        py::module_ commands = py::module_::import("midi_generator.commands");

        py::list python_sequences;
        for (const auto& sequence: sequences) {
            python_sequences.append(to_python_list(sequence));
        }

        py::object result = commands.attr("combine")(python_sequences);

        std::list<midi_generator::Note> combined;
        for(const auto& obj: result)
            combined.emplace_back(
                    obj.attr("pitch").cast<int>(),
                    obj.attr("velocity").cast<int>(),
                    obj.attr("start").cast<double>(),
                    obj.attr("end").cast<double>()
            );
        return combined;

    } catch (py::error_already_set &error) {
        error.discard_as_unraisable(__func__ );
        std::cout << error.what();
        return {};
    }
}

void midi_generator::save_file(std::list<midi_generator::Note> notes, const std::string& filename) {
    try {
        py::module_ commands = py::module_::import("midi_generator.commands");
        py::object result = commands.attr("write_file")(to_python_list(std::move(notes)), filename);
    } catch (py::error_already_set &error) {
        error.discard_as_unraisable(__func__ );
    }
}