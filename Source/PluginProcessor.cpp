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
    _parametersMap.clear();
}

juce::AudioProcessorValueTreeState::ParameterLayout ReverbGUIAudioProcessor::createParameterLayout(juce::AudioProcessorValueTreeState::ParameterLayout &layout){
    
    auto wetParam = std::make_unique<juce::AudioProcessorValueTreeState::Parameter>(ParametersID::wetID, ParametersID::wetID,ParametersID::wetID, juce::NormalisableRange<float>(0.0f,100.0f,1.0f), 50.0f, valueToTextFunction, textToValueFunction);
    
    _parametersMap[ParametersID::wetID] = wetParam.get();
    
    layout.add(std::move(wetParam));
    
    auto dryParam = std::make_unique<juce::AudioProcessorValueTreeState::Parameter>(ParametersID::dryID, ParametersID::dryID,ParametersID::dryID, juce::NormalisableRange<float>(0.0f,100.0f,1.0f), 50.0f, valueToTextFunction, textToValueFunction);
    
    _parametersMap[ParametersID::dryID] = dryParam.get();
    
    layout.add(std::move(dryParam));
    
    
    auto lowPassFilterParam = std::make_unique<juce::AudioProcessorValueTreeState::Parameter>(ParametersID::lowPassFilterID, ParametersID::lowPassFilterID,ParametersID::lowPassFilterID, juce::NormalisableRange<float>(20.0f,20000.0f,1.0f), 20.0f, valueToTextFunction, textToValueFunction);
    
    _parametersMap[ParametersID::lowPassFilterID] = lowPassFilterParam.get();
    
    layout.add(std::move(lowPassFilterParam));
    
    auto highPassFilterParam = std::make_unique<juce::AudioProcessorValueTreeState::Parameter>(ParametersID::highPassFilterID, ParametersID::highPassFilterID,ParametersID::highPassFilterID, juce::NormalisableRange<float>(20.0f,20000.0f,1.0f), 20.0f, valueToTextFunction, textToValueFunction);
    
    _parametersMap[ParametersID::highPassFilterID] = highPassFilterParam.get();
    
    layout.add(std::move(highPassFilterParam));
    
    auto delayParam = std::make_unique<juce::AudioProcessorValueTreeState::Parameter>(ParametersID::delayID, ParametersID::delayID,ParametersID::delayID, juce::NormalisableRange<float>(0.0f,300.0f,1.0f), 10.0f, valueToTextFunction, textToValueFunction);
    
    _parametersMap[ParametersID::delayID] = delayParam.get();
    
    layout.add(std::move(delayParam));
    
    
    auto roomSizeParam = std::make_unique<juce::AudioProcessorValueTreeState::Parameter>(ParametersID::roomSizeID, ParametersID::roomSizeID,ParametersID::roomSizeID, juce::NormalisableRange<float>(0.0f,1.0f,0.1f), 0.1f, valueToTextFunction, textToValueFunction);
    
    _parametersMap[ParametersID::roomSizeID] = roomSizeParam.get();
    
    layout.add(std::move(roomSizeParam));
    
    auto flagLowPassParam = std::make_unique<juce::AudioProcessorValueTreeState::Parameter>(ParametersID::flagLowPassID, ParametersID::flagLowPassID,ParametersID::flagLowPassID, juce::NormalisableRange<float>(0.0f,1.0f,1.0f), 0.0f, valueToTextFunction, textToValueFunction);
    
    _parametersMap[ParametersID::flagLowPassID] = flagLowPassParam.get();
    
    layout.add(std::move(flagLowPassParam));
    
    auto flagHighPassParam = std::make_unique<juce::AudioProcessorValueTreeState::Parameter>(ParametersID::flagHighPassID, ParametersID::flagHighPassID,ParametersID::flagHighPassID, juce::NormalisableRange<float>(0.0f,1.0f,1.0f), 0.0f, valueToTextFunction, textToValueFunction);
    
    _parametersMap[ParametersID::flagHighPassID] = flagHighPassParam.get();
    
    layout.add(std::move(flagHighPassParam));
    
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
    
    _reverbEngine.process(buffer, getTotalNumInputChannels(), getTotalNumOutputChannels());
    
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
    auto state = _apvts->copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void ReverbGUIAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (_apvts->state.getType()))
            _apvts->replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ReverbGUIAudioProcessor();
}

void ReverbGUIAudioProcessor::valueTreePropertyChanged(juce::ValueTree &treeWhosePropertyHasChanged, const juce::Identifier &property){
    
    auto updateId = treeWhosePropertyHasChanged.getProperty("id").toString();
    
    //std::cout << "ID property changed" << updateId << std::endl;
    _reverbEngine.setParameterValue(updateId, _parametersMap[updateId]->get());
    
    
}
