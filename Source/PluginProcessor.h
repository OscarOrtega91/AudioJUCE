/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Engine.h"


//==============================================================================
/**
*/
class ReverbGUIAudioProcessor  : public juce::AudioProcessor,
public juce::ValueTree::Listener
{
public:
    //==============================================================================
    ReverbGUIAudioProcessor();
    ~ReverbGUIAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    Engine _reverbEngine;
    std::map<juce::String, juce::AudioProcessorValueTreeState::Parameter*> _parametersMap;

    static juce::String valueToTextFunction(float x){
        return juce::String(x,2);
    }
    static float textToValueFunction(const juce::String& x){
        return x.getFloatValue();
    }
private:
    //==============================================================================
    
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout(juce::AudioProcessorValueTreeState::ParameterLayout& layout);
    
    std::unique_ptr<juce::AudioProcessorValueTreeState> _apvts;
    
    void valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property) final override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbGUIAudioProcessor)
    
    
    
};
