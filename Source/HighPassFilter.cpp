/*
  ==============================================================================

    HighPassFilter.cpp
    Created: 18 Jun 2023 2:34:01pm
    Author:  Oscar Ortega

  ==============================================================================
*/

#include "HighPassFilter.h"

HighPassFilter::HighPassFilter(){
    
}

HighPassFilter::~HighPassFilter(){
    
}


void HighPassFilter::setCutOffFrequency(int input){
    if(input > 0 && input < 20000)
        _HP_f0 = input;
    
}

int HighPassFilter::getCutOffFrequency(){
    return _HP_f0;
}

void HighPassFilter::setSamplingFrequency(double input){
    Fs = input;
}

void HighPassFilter::prepareToPlay(){
    
    _HP_w0 = 2* juce::MathConstants<float>::pi *_HP_f0/Fs;
    _HP_alpha = sin(_HP_w0)/(2*Q);
    //HPF
    _HP_a0 =   1 + _HP_alpha;
    _HP_a1 =  -2*cos(_HP_w0);
    _HP_a2 =   1 - _HP_alpha;
    _HP_b0 =  (1 + cos(_HP_w0))/2;
    _HP_b1 =   -(1 + cos(_HP_w0));
    _HP_b2 =  (1 + cos(_HP_w0))/2;
    //Normalize
    _HP_a1=_HP_a1/_HP_a0;
    _HP_a2=_HP_a2/_HP_a0;
    _HP_b0=_HP_b0/_HP_a0;
    _HP_b1=_HP_b1/_HP_a0;
    _HP_b2=_HP_b2/_HP_a0;
}

void HighPassFilter::cookingVariables(){
    
    //Cooking variables for High Pass Filter
    _HP_w0= 2*  juce::MathConstants<float>::pi * _HP_f0/Fs;
    _HP_alpha=sin(_HP_w0)/(2*Q);
    //HPF
    
    _HP_a0=1 + _HP_alpha;
    _HP_a1 =-2* cos(_HP_w0);
    _HP_a2= 1- _HP_alpha;

    _HP_b0 =(1+cos(_HP_w0))/2;
    _HP_b1 =-(1+cos(_HP_w0));
    _HP_b2 =(1+cos(_HP_w0))/2;
    //Normalize
    _HP_b0 = _HP_b0 /_HP_a0;
    _HP_b1 = _HP_b1 /_HP_a0;
    _HP_b2 = _HP_b2 /_HP_a0;
    _HP_a1 = _HP_a1 /_HP_a0;
    _HP_a2 = _HP_a2 /_HP_a0;
}

double HighPassFilter::process(double sampleInput){
    
    double out = _HP_b0 * sampleInput + _HP_z1;
    _HP_z1 = _HP_b1 * sampleInput - _HP_a1 * out + _HP_z2;//z1
    _HP_z2 = _HP_b2 * sampleInput - _HP_a2 * out; //z2
    
    return out;

}
