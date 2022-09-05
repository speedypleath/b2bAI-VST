//
// Created by Andrei Gheorghe on 04.09.2022.
//

#include "RunButton.h"

RunButton::RunButton(foleys::MagicGUIBuilder &builder, const juce::ValueTree &node) : GuiItem (builder, node)
{
    setColourTranslation (
            {
                    { "button-color", juce::TextButton::buttonColourId },
                    { "button-on-color", juce::TextButton::buttonOnColourId },
                    { "button-off-text", juce::TextButton::textColourOffId },
                    { "button-on-text", juce::TextButton::textColourOnId }
            });

    addAndMakeVisible (button);
}

void RunButton::update() {
    attachment.reset();
    button.setConnectedEdges(TextButton::ConnectedOnLeft | TextButton::ConnectedOnRight);
    button.setButtonText("Run");

    auto triggerID = getProperty (pOnClick).toString();
    if (triggerID.isNotEmpty())
        button.onClick = getMagicState().getTrigger (triggerID);
}

std::vector<foleys::SettableProperty> RunButton::getSettableProperties() const {
    std::vector<foleys::SettableProperty> props;

    props.push_back ({ configNode, foleys::IDs::parameter, foleys::SettableProperty::Choice, {}, magicBuilder.createParameterMenuLambda() });
    props.push_back ({ configNode, pOnClick, foleys::SettableProperty::Choice, {}, magicBuilder.createTriggerMenuLambda() });
    props.push_back ({ configNode, foleys::IDs::buttonRadioGroup, foleys::SettableProperty::Number, {}, {} });
    props.push_back ({ configNode, foleys::IDs::buttonRadioValue, foleys::SettableProperty::Number, {}, {} });

    return props;
}

juce::Component *RunButton::getWrappedComponent() {
    return &button;
}

const juce::Identifier RunButton::pOnClick { "onClick" };
