//
// Created by Andrei Gheorghe on 21.08.2022.
//

#pragma once
#include "JuceHeader.h"
#include "PianoRollComponent.h"

class PianoRoll: public foleys::GuiItem {
private:
    PianoRollComponent pianoRoll;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PianoRoll)
public:
    FOLEYS_DECLARE_GUI_FACTORY(PianoRoll)

    static const juce::Identifier  pSource;
    static const juce::Identifier  pNoteColor;

    PianoRoll(foleys::MagicGUIBuilder& builder, const juce::ValueTree& node);

    void update() override;

    std::vector<foleys::SettableProperty> getSettableProperties() const override;

    juce::Component* getWrappedComponent() override;
};