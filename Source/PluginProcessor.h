/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DelayLine.h"
#include "LowPassFB.h"
#include "NestedAllPassReverb.h"
#include "AllPassReverbDamping.h"

//==============================================================================
/**
*/
class ReverbGUIAudioProcessor  : public juce::AudioProcessor
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

    int _LP_f0 = 20;
    double Q = 0.707100;
    
    double _LP_w0;
    double _LP_alpha=0.000;
    double _LP_b0=0.0000;
    double _LP_b1=0.000;
    double _LP_b2=0.0000;
    double _LP_a0=0.000;
    double _LP_a1=0.000;
    double _LP_a2=0.000;
    
    double _LeftC_LP_z2=0.000;
    double _LeftC_LP_z1=0.000;
    double _RightC_LP_t2=0.000;
    double _RightC_LP_t1=0.000;
    
    double Fs;
    bool _LP_flag=false;
    
    int _HP_f0 = 20;
    bool _HP_flag=false;
    double _HP_w0;
    double _HP_alpha=0.000;
    double _HP_b0=0.0000;
    double _HP_b1=0.000;
    double _HP_b2=0.0000;
    double _HP_a0=0.000;
    double _HP_a1=0.000;
    double _HP_a2=0.000;
    
    double _LeftC_HP_z2=0.000;
    double _LeftC_HP_z1=0.000;
    double _RightC_HP_t2=0.000;
    double _RightC_HP_t1=0.000;
    
    int _wet=50;
    int _dry=50;
    
    double _roomSize=0.100;

    double _wet_Internal=0.5;
    double _dry_Internal=0.5;
    
    double _roomSize_Internal=0.1000;
    
    std::vector<double> _DelayLineV; // Left 0 Right 1
    std::vector<double> _LPCFV;      // Left 0 Right 1 4 LPCF
    
    std::vector<double> _NestedAPV;  // Left 0 Nested Left 1  Right 2 Nested Right3  3 NestedAP

    std::vector<double> _APDampV;      // Left 0 Right 1 2 APDamp
    DelayLine D1_L;
    DelayLine D1_R;
    
    LowPassFB lpfc1_L;
    LowPassFB lpfc1_R;
    LowPassFB lpfc2_L;
    LowPassFB lpfc2_R;
    LowPassFB lpfc3_L;
    LowPassFB lpfc3_R;
    LowPassFB lpfc4_L;
    LowPassFB lpfc4_R;
    
    
    NestedAllPassReverb NestedAP1_L;
    NestedAllPassReverb NestedAP1_R;
    NestedAllPassReverb NestedAP2_L;
    NestedAllPassReverb NestedAP2_R;
    NestedAllPassReverb NestedAP3_L;
    NestedAllPassReverb NestedAP3_R;
    
    AllPassReverbDamping APDampL1;
    AllPassReverbDamping APDampR1;
    
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbGUIAudioProcessor)
    
    
    
};
