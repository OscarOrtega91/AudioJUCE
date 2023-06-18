/*
  ==============================================================================

    HighPassFilter.h
    Created: 18 Jun 2023 2:34:01pm
    Author:  Oscar Ortega

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class HighPassFilter{
public:
    HighPassFilter();
    ~HighPassFilter();
    
    int getCutOffFrequency();
    void setCutOffFrequency(int input);
    void setSamplingFrequency(double input);
    
    void cookingVariables();
    
    void prepareToPlay();
    
    double process( double sampleInput);
    
private:
    
    int _HP_f0 = 20;
    double Q = 0.707100;
    double _HP_w0;
    double _HP_alpha=0.000;
    double _HP_b0=0.0000;
    double _HP_b1=0.000;
    double _HP_b2=0.0000;
    double _HP_a0=0.000;
    double _HP_a1=0.000;
    double _HP_a2=0.000;
    
    double _HP_z2=0.000;
    double _HP_z1=0.000;
    double Fs;

};
