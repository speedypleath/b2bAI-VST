//
// Created by Andrei Gheorghe on 07.08.2022.
//

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#import "MIDIFileListBox.h"
//==============================================================================
/**
*/
class B2bAIAudioProcessor  : public  foleys::MagicProcessor
{
public:
    //==============================================================================
    B2bAIAudioProcessor();
    ~B2bAIAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const AudioProcessor::BusesLayout& layouts) const override;
#endif
    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;
    void saveMidiFile();
    void loadMidiFile(int index);
    //==============================================================================
    double getTailLengthSeconds() const override;

private:
    //==============================================================================

    // MAGIC GUI: this is a shorthand where the samples to display are fed to
    MIDIFileListBox *midiFileListBox;
    File midiFilesDir;
    AudioProcessorValueTreeState treeState;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (B2bAIAudioProcessor)

    void initialiseBuilder(foleys::MagicGUIBuilder &builder) override;
};
