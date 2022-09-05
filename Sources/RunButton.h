//
// Created by Andrei Gheorghe on 04.09.2022.
//

#pragma once
#include "JuceHeader.h"

class RunButton: public foleys::GuiItem {
public:
    FOLEYS_DECLARE_GUI_FACTORY (RunButton)

    static const juce::Identifier pOnClick;

    RunButton (foleys::MagicGUIBuilder& builder, const juce::ValueTree& node);

    void update() override;

    std::vector<foleys::SettableProperty> getSettableProperties() const override;

    juce::Component* getWrappedComponent() override;

private:
    juce::TextButton button;
    foleys::RadioButtonHandler handler {button, magicBuilder.getRadioButtonManager()};
    std::unique_ptr<juce::ButtonParameterAttachment> attachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RunButton)
};