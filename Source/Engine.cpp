/*
  ==============================================================================

    Engine.cpp
    Created: 19 Jun 2023 8:38:06am
    Author:  Oscar Ortega

  ==============================================================================
*/

#include "Engine.h"

Engine::Engine(){
    
}

Engine::~Engine(){
    
}


double Engine::getSamplingFrequency(){
    return Fs;
}

void Engine::setSamplingFrequency(double samplingFrequency){
    Fs = samplingFrequency;
}

void Engine::changeDelayLength(){
    D1_L.changeDelayLength((int)Fs);
    D1_R.changeDelayLength((int)Fs);
    
    
    lpfc1_L.changeDelayLength((int) Fs);
    lpfc1_R.changeDelayLength((int) Fs);
    lpfc2_L.changeDelayLength((int) Fs);
    lpfc2_R.changeDelayLength((int) Fs);
    lpfc3_L.changeDelayLength((int) Fs);
    lpfc3_R.changeDelayLength((int) Fs);
    lpfc4_L.changeDelayLength((int) Fs);
    lpfc4_R.changeDelayLength((int) Fs);


    
    NestedAP1_L.changeDelayLength((int) Fs);
    NestedAP1_R.changeDelayLength((int) Fs);
    NestedAP2_L.changeDelayLength((int) Fs);
    NestedAP2_R.changeDelayLength((int) Fs);
    NestedAP3_L.changeDelayLength((int) Fs);
    NestedAP3_R.changeDelayLength((int) Fs);
    
    APDampL1.changeDelayLength((int) Fs);
    APDampR1.changeDelayLength((int) Fs);
}


void Engine::initializeEngine(){

    //Set Sampling Frequecy first
    _leftChannelLowPassFilter.setSamplingFrequency(Fs);
    _rightChannelLowPassFilter.setSamplingFrequency(Fs);
    
    _leftChannelHighPassFilter.setSamplingFrequency(Fs);
    _rightChannelHighPassFilter.setSamplingFrequency(Fs);

    //Reset Delay Values
    reset();
    changeDelayLength();
    changeDelayValues();
}

void Engine::reset(){
    // Initialiaze Reverb Delay Values
    _DelayLineV.push_back(10);
    _DelayLineV.push_back(10.53);
    
    _LPCFV.push_back(25.18972);
    _LPCFV.push_back(25.6972);
    _LPCFV.push_back(30.1972);
    _LPCFV.push_back(30.6972);
    _LPCFV.push_back(15.9892);
    _LPCFV.push_back(16.4893);
    _LPCFV.push_back(14.892);
    _LPCFV.push_back(15.3903);
    
    _NestedAPV.push_back(11.23);
    _NestedAPV.push_back(3.1);
    _NestedAPV.push_back(11.7546);
    _NestedAPV.push_back(3.6043);
    _NestedAPV.push_back(10.9560);
    _NestedAPV.push_back(3.456);
    _NestedAPV.push_back(11.4565);
    _NestedAPV.push_back(3.9053);
    _NestedAPV.push_back(7.6365);
    _NestedAPV.push_back(2.5342);
    _NestedAPV.push_back(8.165);
    _NestedAPV.push_back(2.9053);
    
    _APDampV.push_back(180.4492);
    _APDampV.push_back(182.4492);
}


void Engine::changeDelayValues(){
    
    
    D1_L.changeMDelay(_DelayLineV.front());
    D1_R.changeMDelay(_DelayLineV.back());
    
    
    lpfc1_L.changeMDelay(_LPCFV[0]);
    lpfc1_R.changeMDelay(_LPCFV[1]);
    lpfc2_L.changeMDelay(_LPCFV[2]);
    lpfc2_R.changeMDelay(_LPCFV[3]);
    lpfc3_L.changeMDelay(_LPCFV[4]);
    lpfc3_R.changeMDelay(_LPCFV[5]);
    lpfc4_L.changeMDelay(_LPCFV[6]);
    lpfc4_R.changeMDelay(_LPCFV[7]);
    
    
    NestedAP1_L.changeMDelay(_NestedAPV[0]);
    NestedAP1_L.changeMDelayNested(_NestedAPV[1]);
    NestedAP1_R.changeMDelay(_NestedAPV[2]);
    NestedAP1_R.changeMDelayNested(_NestedAPV[3]);
    NestedAP2_L.changeMDelay(_NestedAPV[4]);
    NestedAP2_L.changeMDelayNested(_NestedAPV[5]);
    NestedAP2_R.changeMDelay(_NestedAPV[6]);
    NestedAP2_R.changeMDelayNested(_NestedAPV[7]);
    NestedAP3_L.changeMDelay(_NestedAPV[8]);
    NestedAP3_L.changeMDelayNested(_NestedAPV[9]);
    NestedAP3_R.changeMDelay(_NestedAPV[10]);
    NestedAP3_R.changeMDelayNested(_NestedAPV[11]);
    
    APDampL1.changeMDelay(_APDampV.front());
    APDampR1.changeMDelay(_APDampV.back());
}

void Engine::prepareToPlay(){
    
    // Initialisation Low Pass Filter
    _leftChannelLowPassFilter.setSamplingFrequency(Fs);
    _leftChannelLowPassFilter.prepareToPlay();
    _rightChannelLowPassFilter.setSamplingFrequency(Fs);
    _rightChannelLowPassFilter.prepareToPlay();
    
    // Initialisation High Pass Filter
    _leftChannelHighPassFilter.setSamplingFrequency(Fs);
    _leftChannelHighPassFilter.prepareToPlay();
    _rightChannelHighPassFilter.setSamplingFrequency(Fs);
    _rightChannelHighPassFilter.prepareToPlay();
    
    changeDelayLength();
    changeDelayValues();

    
}


void Engine::process(juce::AudioBuffer<float> &buffer){
    
    auto totalNumInputChannels  = 2;
    auto totalNumOutputChannels = 2;
    double outL,inL,outR,inR;
    double d_procL,d_procR;  //Variables for Delay Output
    double _out_Left_LPCF1,_out_Left_LPCF2,_out_Left_LPCF3; //Left Channel LPCF
    double _out_Left_LPCF4,_out_Left_LPCFTotal; //Left Channel LPCF
    double _out_Right_LPCF1,_out_Right_LPCF2,_out_Right_LPCF3; //Right Channel LPCF
    double _out_Right_LPCF4,_out_Right_LPCFTotal; //Right Channel LPCF
    double _out_NAP_Left1,_out_NAP_Left2,_out_NAP_Left3;
    double _out_NAP_Right1,_out_NAP_Right2,_out_NAP_Right3;
    double _out_APDamp_Right1, _out_APDamp_Left1;
    
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        for(int i=0; i<buffer.getNumSamples();i++){
        if(channel==0){
            //Variable to store Input Signal
            inL=buffer.getSample(channel,i);
            outL=inL;
            
            if(_LP_flag){
                outL= _leftChannelLowPassFilter.process(inL);
            }
            
            // High Pass Filter if Switch is ON
            if(_HP_flag){
                outL = _leftChannelHighPassFilter.process(outL);
            }//End High Pass
            
            // Pre Delay Block
            
            d_procL=D1_L.process(outL);
            
            //End Pre Delay
            
            // LPCF
            // LPCF Left
            // Filters in Parallel from the output of the LP and HP filters
            // All Ouputs will be added at the end by channel
            
            _out_Left_LPCF1=lpfc1_L.process(d_procL);
            _out_Left_LPCF2=lpfc2_L.process(d_procL);
            _out_Left_LPCF3=lpfc3_L.process(d_procL);
            _out_Left_LPCF4=lpfc4_L.process(d_procL);

            _out_Left_LPCFTotal=_out_Left_LPCF1+_out_Left_LPCF2+_out_Left_LPCF3+_out_Left_LPCF4;// Adding all Left outputs
            
            //Nested All Pass
            // Filters in Series, Output of First NEsted All Pass will be the input of the next  Nested All Pass Filter
            
            //LEFT Nested All Pass
            _out_NAP_Left1=NestedAP1_L.process(_out_Left_LPCFTotal);
            _out_NAP_Left2=NestedAP2_L.process(_out_NAP_Left1);
            _out_NAP_Left3= NestedAP3_L.process(_out_NAP_Left2);
            //End Nested All Pass
            
            // All Pass With Damping
            // Filters in Series
            
            // Left AP DAMP
            _out_APDamp_Left1= APDampL1.process(_out_NAP_Left3);
            
            outL=_out_APDamp_Left1;
            
            // Wet and Dry  Mix
            //Left Channel
            outL=outL*_wet_Internal;
            inL=inL *_dry_Internal;
            outL=inL+outL;
            
            
            channelData[i]=outL;


        }
            
        if(channel==1){
            //Variable to store Input Signal
            inR=buffer.getSample(channel,i);
            outR=inR;
            
            if(_LP_flag){
                outR = _rightChannelLowPassFilter.process(inR);
            }
            
            // High Pass Filter if Switch is ON
            if(_HP_flag){
                outR = _rightChannelHighPassFilter.process(outR);

            }//End High Pass
            
            // Pre Delay Block

            d_procR=D1_R.process(outR);
            //End Pre Delay
            
            // LPCF Right
            
            _out_Right_LPCF1= lpfc1_R.process(d_procR);
            _out_Right_LPCF2= lpfc2_R.process(d_procR);
            _out_Right_LPCF3= lpfc3_R.process(d_procR);
            _out_Right_LPCF4= lpfc4_R.process(d_procR);
            
            _out_Right_LPCFTotal=_out_Right_LPCF4+_out_Right_LPCF3+_out_Right_LPCF2+_out_Right_LPCF1; // Adding all Right Outputs
            
            //Nested All Pass
            // Filters in Series, Output of First NEsted All Pass will be the input of the next  Nested All Pass Filter
            
            //Right Nested All Pass
            _out_NAP_Right1=NestedAP1_R.process(_out_Right_LPCFTotal);
            _out_NAP_Right2=NestedAP2_R.process(_out_NAP_Right1);
            _out_NAP_Right3=NestedAP3_R.process(_out_NAP_Right2);
            //End Nested All Pass
            
            // All Pass With Damping
            // Filters in Series


            // Right AP DAMP
            _out_APDamp_Right1= APDampR1.process(_out_NAP_Right3);

            outR=_out_APDamp_Right1;
            
            //Right Channel
            outR=outR*_wet_Internal;
            inR=inR*_dry_Internal;
            outR=outR+inR;
            // End Wet and Dry  Mix
            
            
            channelData[i]=outR;
        }
        }
    }
    
    
}