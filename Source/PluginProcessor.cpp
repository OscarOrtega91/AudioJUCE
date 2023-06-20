/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ReverbGUIAudioProcessor::ReverbGUIAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    _reverbEngine.setSamplingFrequency(getSampleRate());
    _reverbEngine.initializeEngine();
    juce::AudioProcessorValueTreeState::ParameterLayout layout{};
    _apvts = std::make_unique<juce::AudioProcessorValueTreeState>(*this, nullptr, "ReverbModule", createParameterLayout(layout));
    
    _apvts->state.addListener(this);
    
    
}


ReverbGUIAudioProcessor::~ReverbGUIAudioProcessor()
{
    // Clear engine to release resources
}

juce::AudioProcessorValueTreeState::ParameterLayout ReverbGUIAudioProcessor::createParameterLayout(juce::AudioProcessorValueTreeState::ParameterLayout &layout){
    
    auto wetParam = std::make_unique<juce::AudioProcessorValueTreeState::Parameter>(ParametersID::wetID, ParametersID::wetID,ParametersID::wetID, juce::NormalisableRange<float>(0.0f,100.0f,1.0f), 50.0f, valueToTextFunction, textToValueFunction);
    
    _parametersMap["Wet"] = wetParam.get();
    
    layout.add(std::move(wetParam));
    
    return std::move(layout);
}

//==============================================================================
const juce::String ReverbGUIAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ReverbGUIAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ReverbGUIAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ReverbGUIAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ReverbGUIAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ReverbGUIAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ReverbGUIAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ReverbGUIAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ReverbGUIAudioProcessor::getProgramName (int index)
{
    return {};
}

void ReverbGUIAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ReverbGUIAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..

    _reverbEngine.setSamplingFrequency(getSampleRate());
    _reverbEngine.prepareToPlay();

}

void ReverbGUIAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ReverbGUIAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
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

void ReverbGUIAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    //auto val = _parametersMap["Wet"]->get();
    //std::cout<< "Wet Val " << val << std::endl;
    _reverbEngine.process(buffer);
    
}

//==============================================================================
bool ReverbGUIAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ReverbGUIAudioProcessor::createEditor()
{
    return new ReverbGUIAudioProcessorEditor (*this);
}

//==============================================================================
void ReverbGUIAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ReverbGUIAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ReverbGUIAudioProcessor();
}

void ReverbGUIAudioProcessor::valueTreePropertyChanged(juce::ValueTree &treeWhosePropertyHasChanged, const juce::Identifier &property){
    
    const auto updateId = treeWhosePropertyHasChanged.getProperty("id").toString();
    
    std::cout << "ID property changed" << updateId << std::endl;
    
}
