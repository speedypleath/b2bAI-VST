//
// Created by Andrei Gheorghe on 07.08.2022.
//

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "MIDIFileListBox.h"
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
    void loadMidiFile(File file);
    void loadDirectory(const File& file);
    //==============================================================================
    double getTailLengthSeconds() const override;

private:
    //==============================================================================
    MIDIFileListBox *midiFileListBox;
    File midiFilesDir;
    AudioProcessorValueTreeState treeState;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (B2bAIAudioProcessor)
    File getFile(int index);
    void initialiseBuilder(foleys::MagicGUIBuilder &builder) override;
};
