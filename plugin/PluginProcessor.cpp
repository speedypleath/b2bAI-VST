//
// Created by Andrei Gheorghe on 07.08.2022.
//

#include "PluginProcessor.h"
#include "PianoRollComponent.h"

//==============================================================================

namespace IDs
{
    static juce::String mainType  { "mainType" };
    static juce::String mainFreq  { "mainfreq" };
    static juce::String mainLevel { "mainlevel" };
    static juce::String lfoType   { "lfoType" };
    static juce::String lfoFreq   { "lfofreq" };
    static juce::String lfoLevel  { "lfolevel" };
    static juce::String vfoType   { "vfoType" };
    static juce::String vfoFreq   { "vfofreq" };
    static juce::String vfoLevel  { "vfolevel" };

    static juce::Identifier oscilloscope { "oscilloscope" };
}

juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    return layout;
}

//==============================================================================
B2bAIAudioProcessor::B2bAIAudioProcessor()
        : foleys::MagicProcessor (juce::AudioProcessor::BusesProperties()
                                          .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
          treeState (*this, nullptr, "PARAMETERS", createParameterLayout())
{
    FOLEYS_SET_SOURCE_PATH (__FILE__);

    auto file = juce::File::getSpecialLocation (juce::File::currentApplicationFile)
            .getChildFile ("Contents")
            .getChildFile ("Resources")
            .getChildFile ("magic.xml");

    if (file.existsAsFile())
        magicState.setGuiValueTree (file);
    else
        magicState.setGuiValueTree (BinaryData::magic_xml, BinaryData::magic_xmlSize);


    magicState.setApplicationSettingsFile (juce::File::getSpecialLocation (juce::File::userApplicationDataDirectory)
                                                   .getChildFile (ProjectInfo::companyName)
                                                   .getChildFile (ProjectInfo::projectName + juce::String (".settings")));
}

B2bAIAudioProcessor::~B2bAIAudioProcessor()
= default;

void B2bAIAudioProcessor::initialiseBuilder (foleys::MagicGUIBuilder& builder)
{
    builder.registerJUCEFactories();

    builder.registerFactory ("PianoRoll", &PianoRollItem::factory);
}

//==============================================================================

void B2bAIAudioProcessor::setOscillator (juce::dsp::Oscillator<float>& osc, WaveType type)
{
    if (type == WaveType::Sine)
        osc.initialise ([](auto in) { return std::sin (in); });
    else if (type == WaveType::Triangle)
        osc.initialise ([](auto in) { return in / juce::MathConstants<float>::pi; });
    else if (type == WaveType::Square)
        osc.initialise ([](auto in) { return in < 0 ? 1.0f : -1.0f; });
    else
        osc.initialise ([](auto) { return 0.0f; });
}

void B2bAIAudioProcessor::parameterChanged (const juce::String& param, float value)
{

}


void B2bAIAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    const auto numChannels = getTotalNumOutputChannels();

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = juce::uint32 (samplesPerBlock);
    spec.numChannels = juce::uint32 (numChannels);

    // MAGIC GUI: this will setup all internals like MagicPlotSources etc.
    magicState.prepareToPlay (sampleRate, samplesPerBlock);
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

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new B2bAIAudioProcessor();
}