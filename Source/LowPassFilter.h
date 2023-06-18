/*
  ==============================================================================

    LowPassFilter.h
    Created: 18 Jun 2023 2:33:51pm
    Author:  Oscar Ortega

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class LowPassFilter{

public:
    LowPassFilter();
    ~LowPassFilter();

    int getCutOffFrequency();
    void setCutOffFrequency(int input);
    void setSamplingFrequency(double input);
    
    void cookingVariables();
    
    void prepareToPlay();
    
    double process( double sampleInput);
    
private:
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
    
    double _LP_z2=0.000;
    double _LP_z1=0.000;
    
    
    double Fs;

};


