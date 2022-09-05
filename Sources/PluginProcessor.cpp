//
// Created by Andrei Gheorghe on 07.08.2022.
//

#include "PluginProcessor.h"

#include <utility>
#include "MidiSequence.h"
#include "SearchBar.h"
#include "PianoRoll.h"
#include "RunButton.h"
#include "Strings.h"

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

//==============================================================================
namespace logging = boost::log;

juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
    AudioProcessorValueTreeState::ParameterLayout layout;

    auto syncopation  = std::make_unique<juce::AudioParameterInt>(IDs::paramSyncopation,  "Syncopation", 1, 100, 50);
    auto density   = std::make_unique<juce::AudioParameterInt>(IDs::paramDensity,   "Note density",  1, 100, 50);
    auto consonance = std::make_unique<juce::AudioParameterInt>(IDs::paramConsonance, "Consonance", 1, 100, 50);

    auto tempo = std::make_unique<juce::AudioParameterInt>(IDs::paramTempo, "Tempo", 10, 200, 120);
    auto key = std::make_unique<AudioParameterChoice>(IDs::paramKey, "Key", StringArray { "C", "D", "E", "F", "G", "A", "B" }, 1);
    auto mode = std::make_unique<AudioParameterChoice>(IDs::paramMode, "Mode", StringArray { "Major", "Minor", "Dorian", "Phrygian", "Lydian", "Mixolydian", "Locrian" }, 1);

    auto bars = std::make_unique<AudioParameterChoice>(IDs::paramBars, "Bars", StringArray { "4", "8" }, 0);
    auto rate = std::make_unique<AudioParameterChoice>(IDs::paramRate, "Rate", StringArray { "1", "1 / 2", "1 / 4", "1 / 8" }, 2);

    auto syncopationAlgorithm = std::make_unique<AudioParameterChoice>(IDs::paramSyncopationAlgorithm, "Syncopation", StringArray { "WNBD", "Off-Beatness" }, 0);

    auto generate = std::make_unique<juce::AudioProcessorParameterGroup>("generate", "generate", "|",
                                                                      std::move (syncopation),
                                                                      std::move (density),
                                                                      std::move (consonance),
                                                                      std::move (tempo),
                                                                      std::move (key),
                                                                      std::move (mode),
                                                                      std::move (bars),
                                                                      std::move (rate),
                                                                      std::move (syncopationAlgorithm));

    auto pitchRate = std::make_unique<juce::AudioParameterInt>(IDs::paramPitch, "Pitch change", 1, 100, 50);
    auto durationRate = std::make_unique<juce::AudioParameterInt>(IDs::paramSyncopationChange, "Duration change", 1, 100, 50);
    auto velocityRate = std::make_unique<juce::AudioParameterInt>(IDs::paramVelocity, "Velocity change", 1, 100, 50);
    auto consonanceRate = std::make_unique<juce::AudioParameterInt>(IDs::paramConsonanceMutate, "Consonance rate", 1, 100, 50);

    auto mutate = std::make_unique<juce::AudioProcessorParameterGroup>("mutate", "mutate", "|",
                                                                       std::move (pitchRate),
                                                                       std::move (durationRate),
                                                                       std::move (velocityRate),
                                                                       std::move (consonanceRate));

    auto compression = std::make_unique<juce::AudioParameterChoice>(IDs::paramCompression, "Compression algorithm", StringArray { "LZ77", "LZ78", "LZW"}, 0);
    auto continueGroup = std::make_unique<juce::AudioProcessorParameterGroup>("continue", "continue", "|", std::move (compression));

    auto compressionCombine = std::make_unique<juce::AudioParameterChoice>(IDs::paramCompressionCombine, "Compression algorithm", StringArray { "LZ77", "LZ78", "LZW"}, 0);
    auto combine = std::make_unique<juce::AudioProcessorParameterGroup>("combine", "combine", "|", std::move (compressionCombine));
    for (int i = 0; i < 8; i++)
        combine->addChild(std::make_unique<juce::AudioParameterBool>(ParameterID("sequence" + std::to_string(i), 1), "Sequence " + std::to_string(i), false));

    layout.add(std::move (generate));
    layout.add(std::move (mutate));
    layout.add(std::move (continueGroup));
    layout.add(std::move (combine));
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

    magicState.getSettings().setProperty("run", "generate", nullptr);
    magicState.getSettings().setProperty("sequence", "1", nullptr);

    midiFilesDir = File::getSpecialLocation(File::userDocumentsDirectory)
            .getChildFile (ProjectInfo::companyName);

    for (int i = 1; i <= 8; i++) {
        Identifier id("sequence-" + std::to_string(i));
        sequences.add(magicState.createAndAddObject<MidiSequence>(id));
    }

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

    magicState.addTrigger("run", [this] {
        auto run = magicState.getSettings().getProperty("run").toString();
        auto index = magicState.getSettings().getProperty("sequence").toString().getIntValue() - 1;
        BOOST_LOG_TRIVIAL(info) << run << " sequence " << index + 1;

        if(!initialised) {
            pybind11::initialize_interpreter();
            initialised = true;
        }

        if(run.equalsIgnoreCase("generate")) {
            BOOST_LOG_TRIVIAL(info) << getConfiguration();
            sequences[index]->load_notes(midi_generator::generate());
        }

        if(run.equalsIgnoreCase("mutate")) {
            sequences[index]->load_notes(midi_generator::mutate(sequences[index]->to_notes()));
        }

        if(run.equalsIgnoreCase("continue")) {
            sequences[index]->load_notes(midi_generator::continue_sequence(sequences[index]->to_notes()));
        }
    });


    magicState.setApplicationSettingsFile (juce::File::getSpecialLocation (juce::File::userApplicationDataDirectory)
                                                   .getChildFile (ProjectInfo::companyName)
                                                   .getChildFile (ProjectInfo::projectName + juce::String (".settings")));

    magicState.getSettings().setProperty("path", midiFilesDir.getFullPathName(), nullptr);
}

B2bAIAudioProcessor::~B2bAIAudioProcessor()
= default;

void B2bAIAudioProcessor::initialiseBuilder (foleys::MagicGUIBuilder& builder)
{
    builder.registerJUCEFactories();

    builder.registerFactory ("PianoRoll", &PianoRoll::factory);
    builder.registerFactory ("SearchBar", &SearchBar::factory);
    builder.registerFactory ("RunButton", &RunButton::factory);
}

//==============================================================================

void B2bAIAudioProcessor::prepareToPlay (double, int) {

}

void B2bAIAudioProcessor::releaseResources() {
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
    auto path = magicState.getSettings().getProperty("path").toString().toStdString();
    path += '/';
    auto file_name  = magicState.getSettings().getProperty("file_name").toString().toStdString();
    path += file_name;
    path += ".mid";
    auto index = magicState.getSettings().getProperty("sequence").toString().getIntValue() - 1;
    BOOST_LOG_TRIVIAL(info) << "Save at path: " << path;
    midi_generator::save_file(sequences[index]->to_notes(), path);
}

void B2bAIAudioProcessor::loadMidiFile(const File& file) {
    if(file.existsAsFile()) {
        auto index = magicState.getSettings().getProperty("sequence").toString().getIntValue() - 1;
        sequences[index]->load(file);
    }
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

midi_generator::Configuration B2bAIAudioProcessor::getConfiguration() {
    midi_generator::Configuration configuration{};

    configuration.syncopation = static_cast<float>(dynamic_cast<AudioParameterInt *>(treeState.getParameter(IDs::paramSyncopation.getParamID()))->get()) / 100;
    configuration.density = static_cast<float>(dynamic_cast<AudioParameterInt *>(treeState.getParameter(IDs::paramDensity.getParamID()))->get()) / 100;
    configuration.consonance = static_cast<float>(dynamic_cast<AudioParameterInt *>(treeState.getParameter(IDs::paramConsonance.getParamID()))->get()) / 100;

    configuration.scale = midi_generator::Scale
            (dynamic_cast<AudioParameterChoice *>(treeState.getParameter(IDs::paramKey.getParamID()))->getIndex(),
             dynamic_cast<AudioParameterChoice *>(treeState.getParameter(IDs::paramKey.getParamID()))->getIndex());

    configuration.bars = dynamic_cast<AudioParameterChoice *>(treeState.getParameter(IDs::paramBars.getParamID()))->getCurrentChoiceName().getIntValue();
    juce::String rate = dynamic_cast<AudioParameterChoice *>(treeState.getParameter(IDs::paramRate.getParamID()))->getCurrentChoiceName();

    configuration.rate = 1.f / (rate.indexOf("/") == -1 ? 1 : static_cast<float>(rate.getTrailingIntValue()));

    return configuration;
}
//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new B2bAIAudioProcessor();
}