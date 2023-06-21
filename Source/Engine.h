/*
  ==============================================================================

    Engine.h
    Created: 19 Jun 2023 8:38:06am
    Author:  Oscar Ortega

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DelayLine.h"
#include "LowPassFB.h"
#include "NestedAllPassReverb.h"
#include "AllPassReverbDamping.h"
#include "LowPassFilter.h"
#include "HighPassFilter.h"

namespace ParametersID{

 const juce::String wetID="Wet";
 const juce::String dryID="Dry";
 const juce::String lowPassFilterID="Low Pass Filter";
 const juce::String highPassFilterID="High Pass Filter";
 const juce::String delayID="Delay";
 const juce::String roomSizeID="Room Size";
 const juce::String flagLowPassID="Flag Low Pass Filter";
 const juce::String flagHighPassID="Flag High Pass Filter";
}


class Engine
{
public:
    Engine();
    ~Engine();
    void initializeEngine();
    double getSamplingFrequency();
    void setSamplingFrequency(double samplingFrequency);
    void process(juce::AudioBuffer<float>& buffer , int inputChannels, int outputChannels);
    void prepareToPlay();
    void setParameterValue(juce::String& id , double val);
    double getParameterValue(const juce::String& id);





private:
    void changeDelayValues();
    void changeDelayLength();

    void reset(); // Reset Delay Values
    void resetDelay();
    void setRoomSizeValue(double input);
    void changeRoomSize();
    
    bool _LP_flag=false;
    
    bool _HP_flag=false;
    double Fs; //Sampling Frequency

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
    
    LowPassFilter _leftChannelLowPassFilter;
    LowPassFilter _rightChannelLowPassFilter;
    
    HighPassFilter _leftChannelHighPassFilter;
    HighPassFilter _rightChannelHighPassFilter;
    
};
