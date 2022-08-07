//
// Created by Andrei Gheorghe on 07.08.2022.
//

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class B2bAIAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    explicit B2bAIAudioProcessorEditor (B2bAIAudioProcessor&);
    ~B2bAIAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    B2bAIAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (B2bAIAudioProcessorEditor)
};
