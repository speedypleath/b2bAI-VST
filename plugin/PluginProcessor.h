//
// Created by Andrei Gheorghe on 07.08.2022.
//

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/**
*/
class B2bAIAudioProcessor  : public  foleys::MagicProcessor,
                             private juce::AudioProcessorValueTreeState::Listener
{
public:
    enum WaveType
    {
        None = 0,
        Sine,
        Triangle,
        Square
    };

    //==============================================================================
    B2bAIAudioProcessor();
    ~B2bAIAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const juce::AudioProcessor::BusesLayout& layouts) const override;
#endif

    void parameterChanged (const juce::String& param, float value) override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    double getTailLengthSeconds() const override;

private:
    //==============================================================================

    void setOscillator (juce::dsp::Oscillator<float>& osc, WaveType type);

    juce::AudioProcessorValueTreeState treeState;
    // MAGIC GUI: this is a shorthand where the samples to display are fed to
    foleys::MagicPlotSource*    oscilloscope = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (B2bAIAudioProcessor)

    void initialiseBuilder(foleys::MagicGUIBuilder &builder) override;
};
