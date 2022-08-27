//
// Created by Andrei Gheorghe on 20.08.2022.
//
#include "JuceHeader.h"
#pragma once

class SearchBar : public foleys::GuiItem
{
private:
    juce::Label label;
    Label::Listener *listener = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SearchBar)

public:
    FOLEYS_DECLARE_GUI_FACTORY (SearchBar)

    static const juce::Identifier  pText;
    static const juce::Identifier  pJustification;
    static const juce::Identifier  pFontSize;
    static const juce::Identifier  pDestination;

    SearchBar (foleys::MagicGUIBuilder& builder, const juce::ValueTree& node);

    void update() override;

    std::vector<foleys::SettableProperty> getSettableProperties() const override;

    juce::Component* getWrappedComponent() override;
};