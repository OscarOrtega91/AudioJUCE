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
    Fs=getSampleRate();
    
    // Initialiaze Reverb Filters
    _DelayLineV.push_back(10);
    _DelayLineV.push_back(10.53);
    
    D1_L.changeDelayLength((int)Fs);
    D1_R.changeDelayLength((int)Fs);
    
    D1_L.changeMDelay(_DelayLineV.front());
    D1_R.changeMDelay(_DelayLineV.back());
    
    
    _LPCFV.push_back(25.18972);
    _LPCFV.push_back(25.6972);
    _LPCFV.push_back(30.1972);
    _LPCFV.push_back(30.6972);
    _LPCFV.push_back(15.9892);
    _LPCFV.push_back(16.4893);
    _LPCFV.push_back(14.892);
    _LPCFV.push_back(15.3903);
    
    lpfc1_L.changeDelayLength((int) Fs);
    lpfc1_R.changeDelayLength((int) Fs);
    lpfc2_L.changeDelayLength((int) Fs);
    lpfc2_R.changeDelayLength((int) Fs);
    lpfc3_L.changeDelayLength((int) Fs);
    lpfc3_R.changeDelayLength((int) Fs);
    lpfc4_L.changeDelayLength((int) Fs);
    lpfc4_R.changeDelayLength((int) Fs);
    
    lpfc1_L.changeMDelay(_LPCFV[0]);
    lpfc1_R.changeMDelay(_LPCFV[1]);
    lpfc2_L.changeMDelay(_LPCFV[2]);
    lpfc2_R.changeMDelay(_LPCFV[3]);
    lpfc3_L.changeMDelay(_LPCFV[4]);
    lpfc3_R.changeMDelay(_LPCFV[5]);
    lpfc4_L.changeMDelay(_LPCFV[6]);
    lpfc4_R.changeMDelay(_LPCFV[7]);
    
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
    
    NestedAP1_L.changeDelayLength((int) Fs);
    NestedAP1_R.changeDelayLength((int) Fs);
    NestedAP2_L.changeDelayLength((int) Fs);
    NestedAP2_R.changeDelayLength((int) Fs);
    NestedAP3_L.changeDelayLength((int) Fs);
    NestedAP3_R.changeDelayLength((int) Fs);
    
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
    
    
    _APDampV.push_back(180.4492);
    _APDampV.push_back(182.4492);
    APDampL1.changeDelayLength((int) Fs);
    APDampR1.changeDelayLength((int) Fs);
    APDampL1.changeMDelay(_APDampV.front());
    APDampR1.changeMDelay(_APDampV.back());

}

ReverbGUIAudioProcessor::~ReverbGUIAudioProcessor()
{
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

    Fs=sampleRate;
    D1_L.changeDelayLength((int)Fs);
    D1_R.changeDelayLength((int)Fs);
    
    D1_L.changeMDelay(_DelayLineV.front());
    D1_R.changeMDelay(_DelayLineV.back());
    
    //Low Pass Filter
    _LP_w0 = 2* juce::MathConstants<float>::pi *_LP_f0/sampleRate;
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
    
    //High Pass Filter
    _HP_w0 = 2* juce::MathConstants<float>::pi *_HP_f0/sampleRate;
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
    
    
    //LPFB
    lpfc1_L.changeDelayLength((int) Fs);
    lpfc1_R.changeDelayLength((int) Fs);
    lpfc2_L.changeDelayLength((int) Fs);
    lpfc2_R.changeDelayLength((int) Fs);
    lpfc3_L.changeDelayLength((int) Fs);
    lpfc3_R.changeDelayLength((int) Fs);
    lpfc4_L.changeDelayLength((int) Fs);
    lpfc4_R.changeDelayLength((int) Fs);
    
    lpfc1_L.changeMDelay(_LPCFV[0]);
    lpfc1_R.changeMDelay(_LPCFV[1]);
    lpfc2_L.changeMDelay(_LPCFV[2]);
    lpfc2_R.changeMDelay(_LPCFV[3]);
    lpfc3_L.changeMDelay(_LPCFV[4]);
    lpfc3_R.changeMDelay(_LPCFV[5]);
    lpfc4_L.changeMDelay(_LPCFV[6]);
    lpfc4_R.changeMDelay(_LPCFV[7]);
    
    
    
    NestedAP1_L.changeDelayLength((int) Fs);
    NestedAP1_R.changeDelayLength((int) Fs);
    NestedAP2_L.changeDelayLength((int) Fs);
    NestedAP2_R.changeDelayLength((int) Fs);
    NestedAP3_L.changeDelayLength((int) Fs);
    NestedAP3_R.changeDelayLength((int) Fs);
    
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
    
    APDampL1.changeDelayLength((int) Fs);
    APDampR1.changeDelayLength((int) Fs);
    APDampL1.changeMDelay(_APDampV.front());
    APDampR1.changeMDelay(_APDampV.back());
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
    double outL,inL,outR,inR;
    double d_procL,d_procR;  //Variables for Delay Output
    double tempL, tempR;
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
                outL = _LP_b0*inL + _LeftC_LP_z1;
                _LeftC_LP_z1 = _LP_b1*inL - _LP_a1*outL + _LeftC_LP_z2;//z1
                _LeftC_LP_z2 = _LP_b2*inL - _LP_a2*outL; //z2
            }
            
            // High Pass Filter if Switch is ON
            if(_HP_flag){
                tempL=outL;
                outL = _HP_b0*tempL + _LeftC_HP_z1;
                _LeftC_HP_z1 = _HP_b1*tempL - _HP_a1*outL + _LeftC_HP_z2;//z1
                _LeftC_HP_z2 = _HP_b2*tempL - _HP_a2*outL; //z2
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
                outR=_LP_b0*inR + _RightC_LP_t1;
                _RightC_LP_t1 = _LP_b1 * inR - _LP_a1 * outR + _RightC_LP_t2;//t1
                _RightC_LP_t2 = _LP_b2 * inR - _LP_a2 * outR;
            }
            
            // High Pass Filter if Switch is ON
            if(_HP_flag){
                tempR=outR;
                outR=_HP_b0*tempR + _RightC_HP_t1;
                _RightC_HP_t1 = _HP_b1 * tempR - _HP_a1 * outR + _RightC_HP_t2;//t1
                _RightC_HP_t2 = _HP_b2 * tempR - _HP_a2 * outR;
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
