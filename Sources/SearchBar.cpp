//
// Created by Andrei Gheorghe on 20.08.2022.
//

#include "SearchBar.h"
#include "MidiFileListBox.h"

SearchBar::SearchBar (foleys::MagicGUIBuilder& builder, const juce::ValueTree& node) : GuiItem (builder, node) {
    label.setEditable(true);
    setColourTranslation (
        {
        { "label-background",         juce::Label::backgroundColourId },
        { "label-outline",            juce::Label::outlineColourId },
        { "label-text",               juce::Label::textColourId },
        { "label-editing-background", juce::Label::backgroundWhenEditingColourId },
        { "label-editing-outline",    juce::Label::outlineWhenEditingColourId },
        { "label-editing-text",       juce::Label::textWhenEditingColourId }
    });

    addAndMakeVisible (label);
}

void SearchBar::update() {
    label.setEditable(true);
    label.setText (magicBuilder.getStyleProperty (pText, configNode), juce::sendNotification);

    auto justifications = foleys::makeJustificationsChoices();
    auto justification = getProperty (pJustification).toString();

    if (justification.isNotEmpty())
        label.setJustificationType (juce::Justification (justifications.getWithDefault (justification, juce::Justification::centredLeft)));
    else
        label.setJustificationType (juce::Justification::centredLeft);

    label.setFont (juce::Font (getProperty (pFontSize)));

    auto destination = getProperty (pDestination).toString();

    if(destination.isNotEmpty())
        label.addListener(getMagicState().getObjectWithType<MidiFileListBox>(destination));
}

std::vector<foleys::SettableProperty> SearchBar::getSettableProperties() const {
    std::vector<foleys::SettableProperty> props;
    props.push_back ({ configNode, pText, foleys::SettableProperty::Text, {}, {} });
    props.push_back ({ configNode, pJustification, foleys::SettableProperty::Choice, {}, magicBuilder.createChoicesMenuLambda (foleys::getAllKeyNames (foleys::makeJustificationsChoices())) });
    props.push_back ({ configNode, pFontSize, foleys::SettableProperty::Number, {}, {} });
    props.push_back ({ configNode, pDestination, foleys::SettableProperty::Choice, {}, magicBuilder.createObjectsMenuLambda<MidiFileListBox>() });
    return props;
}

juce::Component* SearchBar::getWrappedComponent() {
    return &label;
}

const juce::Identifier  SearchBar::pText            { "text" };
const juce::Identifier  SearchBar::pJustification   { "justification" };
const juce::Identifier  SearchBar::pFontSize        { "font-size" };
const juce::Identifier  SearchBar::pDestination     { "destination" };