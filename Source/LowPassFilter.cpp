/*
  ==============================================================================

    LowPassFilter.cpp
    Created: 18 Jun 2023 2:33:51pm
    Author:  Oscar Ortega

  ==============================================================================
*/

#include "LowPassFilter.h"

LowPassFilter::LowPassFilter(){
    
}

LowPassFilter::~LowPassFilter(){
    
}

void LowPassFilter::setCutOffFrequency(int input){
    if(input > 0 && input < 20000)
        _LP_f0 = input;
    
}

int LowPassFilter::getCutOffFrequency(){
    return _LP_f0;
}

void LowPassFilter::setSamplingFrequency(double input){
    Fs = input;
}

void LowPassFilter::prepareToPlay(){
    
    //Low Pass Filter
    _LP_w0 = 2* juce::MathConstants<float>::pi *_LP_f0/Fs;
    _LP_alpha = sin(_LP_w0)/(2*Q);
    //LPF
    _LP_a0 =   1 + _LP_alpha;
    _LP_a1 =  -2*cos(_LP_w0);
    _LP_a2 =   1 - _LP_alpha;
    _LP_b0 =  (1 - cos(_LP_w0))/2;
    _LP_b1 =   1 - cos(_LP_w0);
    _LP_b2 =  (1 - cos(_LP_w0))/2;
    //Normalize
    _LP_a1=_LP_a1/_LP_a0;
    _LP_a2=_LP_a2/_LP_a0;
    _LP_b0=_LP_b0/_LP_a0;
    _LP_b1=_LP_b1/_LP_a0;
    _LP_b2=_LP_b2/_LP_a0;
}


void LowPassFilter::cookingVariables(){
    
    //Cooking variables for Low Pass Filter
    //_LP_f0=_lpSlider.getValue();
    _LP_w0= 2*  juce::MathConstants<float>::pi * _LP_f0/Fs;
    _LP_alpha=sin(_LP_w0)/(2*Q);
    
    //LPF
    _LP_a0 = 1 + _LP_alpha;
    _LP_a1 = -2 * cos(_LP_w0);
    _LP_a2 = 1 - _LP_alpha;

    _LP_b0 = (1-cos(_LP_w0))/2;
    _LP_b1 = 1-cos(_LP_w0);
    _LP_b2 = (1-cos(_LP_w0))/2;
    
    //Normalize
    _LP_b0 = _LP_b0 /_LP_a0;
    _LP_b1 = _LP_b1 /_LP_a0;
    _LP_b2 = _LP_b2 /_LP_a0;
    _LP_a1 = _LP_a1 /_LP_a0;
    _LP_a2 = _LP_a2 /_LP_a0;
}

double LowPassFilter::process(double sampleInput){
    
    double out = _LP_b0 * sampleInput + _LP_z1;
    _LP_z1 = _LP_b1 * sampleInput - _LP_a1 * out + _LP_z2;//z1
    _LP_z2 = _LP_b2 * sampleInput - _LP_a2 * out; //z2
    
    return out;
}
