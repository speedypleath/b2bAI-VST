//
// Created by Andrei Gheorghe on 07.08.2022.
//

#include "PluginProcessor.h"
#include "PianoRollComponent.h"

//==============================================================================

namespace IDs
{
    static String paramSyncopation {"syncopation" };
    static String paramDensity { "note density" };
    static String paramBars { "number of bars" };
    static String paramScale { "scale" };
}

juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    return layout;
}

//==============================================================================
B2bAIAudioProcessor::B2bAIAudioProcessor()
        : treeState (*this, nullptr, "PARAMETERS", createParameterLayout())
{
    FOLEYS_SET_SOURCE_PATH (__FILE__);

    auto file = File::getSpecialLocation (File::currentApplicationFile)
            .getChildFile ("Contents")
            .getChildFile ("Resources")
            .getChildFile ("magic.xml");

    if (file.existsAsFile())
        magicState.setGuiValueTree (file);
    else
        magicState.setGuiValueTree (BinaryData::magic_xml, BinaryData::magic_xmlSize);

    midiFileListBox = magicState.createAndAddObject<MIDIFileListBox>("midi_files");

    midiFileListBox->onSelectionChanged = [&](int number) {
        loadMidiFile(number);
    };

    magicState.addTrigger("save_file", [this] {
        saveMidiFile();
    });

    magicState.setApplicationSettingsFile (juce::File::getSpecialLocation (juce::File::userApplicationDataDirectory)
                                                   .getChildFile (ProjectInfo::companyName)
                                                   .getChildFile (ProjectInfo::projectName + juce::String (".settings")));

    midiFilesDir = File::getSpecialLocation(File::userDocumentsDirectory)
            .getChildFile (ProjectInfo::companyName)
            .getChildFile("midi_files");

    magicState.setPlayheadUpdateFrequency (30);
}

B2bAIAudioProcessor::~B2bAIAudioProcessor()
= default;

void B2bAIAudioProcessor::initialiseBuilder (foleys::MagicGUIBuilder& builder)
{
    builder.registerJUCEFactories();

    builder.registerFactory ("PianoRoll", &PianoRollItem::factory);
}

//==============================================================================

void B2bAIAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock) {

}

void B2bAIAudioProcessor::releaseResources()
{
}

void B2bAIAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    ignoreUnused (midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    for (int i=1; i < getTotalNumOutputChannels(); ++i)
        buffer.copyFrom (i, 0, buffer.getReadPointer (0), buffer.getNumSamples());

}

//==============================================================================
#ifndef JucePlugin_PreferredChannelConfigurations
bool B2bAIAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

//==============================================================================
double B2bAIAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

void B2bAIAudioProcessor::saveMidiFile() {
    midiFilesDir = File::getSpecialLocation(File::userDocumentsDirectory)
            .getChildFile (ProjectInfo::companyName)
            .getChildFile("midi_files");

    midiFilesDir.getChildFile("Midi " + juce::String (midiFileListBox->getNumRows() + 1) + ".mid").create();
    midiFileListBox->setFileDir(midiFilesDir);
}

void B2bAIAudioProcessor::loadMidiFile(int number) {


    if(number == 0)
        midiFilesDir = midiFilesDir.getParentDirectory();
    else {
        number -= 1;
        auto directories = midiFilesDir.findChildFiles(File::findDirectories, false, "*");

        if (number < directories.size())
            midiFilesDir = directories[number];

        number -= directories.size();
        auto midiFiles = midiFilesDir.findChildFiles(File::findFiles, false, "*.mid");
    }
    midiFileListBox->setFileDir(midiFilesDir);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new B2bAIAudioProcessor();
}