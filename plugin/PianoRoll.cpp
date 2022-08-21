//
// Created by Andrei Gheorghe on 21.08.2022.
//

#include "PianoRoll.h"

PianoRoll::PianoRoll(foleys::MagicGUIBuilder& builder, const juce::ValueTree& node)
    : foleys::GuiItem(builder, node),
    pianoRoll(getMagicState().getKeyboardState(), juce::MidiKeyboardComponent::verticalKeyboardFacingRight) {
    setColourTranslation ({
          { "note-colour", PianoRollComponent::noteColour                                     },
          { "white-note-color",      juce::MidiKeyboardComponent::whiteNoteColourId           },
          { "black-note-color",      juce::MidiKeyboardComponent::blackNoteColourId           },
          { "key-separator-line-color", juce::MidiKeyboardComponent::keySeparatorLineColourId },
          { "mouse-over-color",      juce::MidiKeyboardComponent::mouseOverKeyOverlayColourId },
          { "key-down-color",        juce::MidiKeyboardComponent::keyDownOverlayColourId      },
    });

    addAndMakeVisible(pianoRoll);
}

void PianoRoll::update() {
    auto sequence = getProperty (pSource).toString();

    if(sequence.isNotEmpty())
        pianoRoll.setMidiSequence(getMagicState().getObjectWithType<MidiSequence>(sequence));
}

std::vector<foleys::SettableProperty> PianoRoll::getSettableProperties() const {
    std::vector<foleys::SettableProperty> props;
    props.push_back ({ configNode, pSource, foleys::SettableProperty::Choice, {}, magicBuilder.createObjectsMenuLambda<MidiSequence>() });
    return props;
}

juce::Component* PianoRoll::getWrappedComponent() {
    return &pianoRoll;
}

const juce::Identifier  PianoRoll::pSource { "source" };