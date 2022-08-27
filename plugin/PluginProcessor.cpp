//
// Created by Andrei Gheorghe on 07.08.2022.
//

#include "PluginProcessor.h"

#include <utility>
#include "MidiSequence.h"
#include "SearchBar.h"
#include "PianoRoll.h"

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

//==============================================================================
namespace logging = boost::log;
namespace IDs
{
    static ParameterID paramSyncopation {"syncopation", 1 };
    static ParameterID paramDensity { "density", 1 };
    static ParameterID paramBars { "harmony", 1 };
    static ParameterID paramScale { "roughness", 1 };
    static ParameterID paramTempo { "tempo", 1 };
    static ParameterID paramKey { "key", 1 };
    static ParameterID paramMode { "mode", 1 };
}

juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
    AudioProcessorValueTreeState::ParameterLayout layout;

    auto attack  = std::make_unique<juce::AudioParameterFloat>(IDs::paramSyncopation,  "Syncopation",  juce::NormalisableRange<float> (0.001f, 0.5f, 0.01f), 0.10f);
    auto decay   = std::make_unique<juce::AudioParameterFloat>(IDs::paramDensity,   "Note density",   juce::NormalisableRange<float> (0.001f, 0.5f, 0.01f), 0.10f);
    auto sustain = std::make_unique<juce::AudioParameterFloat>(IDs::paramBars, "Harmony", juce::NormalisableRange<float> (0.0f,   1.0f, 0.01f), 1.0f);
    auto release = std::make_unique<juce::AudioParameterFloat>(IDs::paramScale, "Roughness", juce::NormalisableRange<float> (0.001f, 0.5f, 0.01f), 0.10f);

    auto group = std::make_unique<juce::AudioProcessorParameterGroup>("adsr", "ADRS", "|",
                                                                      std::move (attack),
                                                                      std::move (decay),
                                                                      std::move (sustain),
                                                                      std::move (release));

    auto tempo = std::make_unique<juce::AudioParameterInt>(IDs::paramTempo, "Tempo", 10, 200, 120);
    auto key = std::make_unique<AudioParameterChoice>(IDs::paramKey, "Key", StringArray {"A", "B", "C", "D", "F", "G"}, 1);
    auto mode = std::make_unique<AudioParameterChoice>(IDs::paramMode, "Mode", StringArray {"Major", "Minor", "Dorian", "Phrygian", "Lydian", "Mixolydian", "Locrian"}, 1);
    
    auto scale = std::make_unique<juce::AudioProcessorParameterGroup>("scale", "Scale", "|",
                                                                      std::move (tempo),
                                                                      std::move (key),
                                                                      std::move (mode));

    layout.add(std::move (scale));
    layout.add (std::move (group));
    return layout;
}

//==============================================================================
B2bAIAudioProcessor::B2bAIAudioProcessor()
        : treeState (*this, nullptr, "PARAMETERS", createParameterLayout())
{
    FOLEYS_SET_SOURCE_PATH (__FILE__)

    logging::core::get()->set_filter(logging::trivial::severity >= logging::trivial::info);

    auto resFile = File::getSpecialLocation (File::currentApplicationFile)
            .getChildFile ("Contents")
            .getChildFile ("Resources")
            .getChildFile ("magic.xml");

    if (resFile.existsAsFile())
        magicState.setGuiValueTree (resFile);
    else
        magicState.setGuiValueTree (BinaryData::magic_xml, BinaryData::magic_xmlSize);


    midiFilesDir = File::getSpecialLocation(File::userDocumentsDirectory)
            .getChildFile (ProjectInfo::companyName);

    midiSequence = magicState.createAndAddObject<MidiSequence>("sequence");

    midiFileListBox = magicState.createAndAddObject<MidiFileListBox>("filetree");

    midiFileListBox->update = [&](const String& text) {
        updateListBox(text);
    };

    midiFileListBox->onSelectionChanged = [&](const File& file) {
        loadMidiFile(file);
    };

    midiFileListBox->onDoubleClick = [&](const File& file) {
        loadDirectory(file);
    };

    magicState.addTrigger("save_file", [this] {
        saveMidiFile();
    });

    magicState.setApplicationSettingsFile (juce::File::getSpecialLocation (juce::File::userApplicationDataDirectory)
                                                   .getChildFile (ProjectInfo::companyName)
                                                   .getChildFile (ProjectInfo::projectName + juce::String (".settings")));

    magicState.getSettings().setProperty("path", midiFilesDir.getFullPathName(), nullptr);

    magicState.setPlayheadUpdateFrequency (30);
}

B2bAIAudioProcessor::~B2bAIAudioProcessor()
= default;

void B2bAIAudioProcessor::initialiseBuilder (foleys::MagicGUIBuilder& builder)
{
    builder.registerJUCEFactories();

    builder.registerFactory ("PianoRoll", &PianoRoll::factory);
    builder.registerFactory ("SearchBar", &SearchBar::factory);
}

//==============================================================================

void B2bAIAudioProcessor::prepareToPlay (double, int) {

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
    midiSequence->save(midiFilesDir);
}

void B2bAIAudioProcessor::loadMidiFile(const File& file) {
    if(file.existsAsFile())
        midiSequence->load(file);
}

void B2bAIAudioProcessor::loadDirectory(const File& file) {

    BOOST_LOG_TRIVIAL(info) << "Change directory: " << file.getFullPathName() << std::endl;
    if(file.isDirectory()) {
        magicState.getSettings().setProperty("path", file.getFullPathName(), nullptr);
        midiFilesDir = file;
    }
}

void B2bAIAudioProcessor::updateListBox(const juce::String& text) {
    magicState.getSettings().setProperty("text", text, nullptr);
}

File B2bAIAudioProcessor::getFile(int index) {
    File file;
    if(index == 0)
        file = midiFilesDir.getParentDirectory();
    else {
        index -= 1;
        auto directories = midiFilesDir.findChildFiles(File::findDirectories, false, "*");

        if (index < directories.size())
            file = directories[index];
        else {
            index -= directories.size();
            auto files = midiFilesDir.findChildFiles(File::findFiles, false, "*.mid");

            file = files[index];
        }
    }

    return file;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new B2bAIAudioProcessor();
}