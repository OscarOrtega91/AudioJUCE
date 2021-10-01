/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ReverbGUIAudioProcessorEditor::ReverbGUIAudioProcessorEditor (ReverbGUIAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    _lpSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    _lpSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 40);
    _lpSlider.setRange(20.00, 20000.00,1.0);
    _lpSlider.setSkewFactorFromMidPoint(1200);
    _lpSlider.addListener(this);
    _lpSlider.setTextValueSuffix(" Hz");
    _lpSlider.setValue(audioProcessor._LP_f0);

    addAndMakeVisible(_lpSlider);
    _lpButton.setButtonText("Active");
    _lpButton.setToggleState(audioProcessor._LP_flag, juce::dontSendNotification);
    
    getLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colours::cyan);
    getLookAndFeel().setColour(juce::ToggleButton::tickColourId, juce::Colours::cornflowerblue);
    _lpButton.addListener(this);
    
    
    
    addAndMakeVisible(_lpButton);

    addAndMakeVisible (_lpLabel);
    _lpLabel.setText ("Low Pass ", juce::dontSendNotification);
    _lpLabel.setJustificationType(juce::Justification::centred);

    _hpSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    _hpSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 40);
    _hpSlider.setRange(20.00, 20000.00,1.0);
    _hpSlider.setSkewFactorFromMidPoint(1200);
    _hpSlider.setTextValueSuffix(" Hz");

    _hpSlider.addListener(this);
    _hpSlider.setValue(audioProcessor._HP_f0);

    addAndMakeVisible(_hpSlider);
    _hpButton.setButtonText("Active");
    
    _hpButton.addListener(this);
    
    _hpButton.setToggleState(audioProcessor._HP_flag, juce::dontSendNotification);

    addAndMakeVisible(_hpButton);
    
    addAndMakeVisible (_hpLabel);
    _hpLabel.setText ("High Pass ", juce::dontSendNotification);
    _hpLabel.setJustificationType(juce::Justification::centred);
    
    _wetSlider.setRange(0, 100,1);
    _wetSlider.addListener(this);
    addAndMakeVisible(_wetSlider);
    _wetSlider.setTextValueSuffix(" %");
    _wetSlider.setValue(audioProcessor._wet);
    
    addAndMakeVisible (_wetLabel);
    _wetLabel.setText ("Wet % ", juce::dontSendNotification);
    _wetLabel.attachToComponent (&_wetSlider, true);
    
    
    _drySlider.setRange(0, 100,1);
    _drySlider.addListener(this);
    addAndMakeVisible(_drySlider);
    _drySlider.setTextValueSuffix(" %");
    _drySlider.setValue(audioProcessor._dry);
    
    addAndMakeVisible (_dryLabel);
    _dryLabel.setText ("Dry % ", juce::dontSendNotification);
    _dryLabel.attachToComponent (&_drySlider, true);

    _roomSizeSlider.setRange(0.00, 1.00,0.1);
    _roomSizeSlider.setValue(0.50);
    _roomSizeSlider.addListener(this);
    addAndMakeVisible(_roomSizeSlider);
    
    addAndMakeVisible (_roomSizeLabel);
    _roomSizeLabel.setText ("Room Size ", juce::dontSendNotification);
    _roomSizeLabel.attachToComponent (&_roomSizeSlider, true);

    _preDelaySlider.setRange(0, 300,1);
    _preDelaySlider.addListener(this);
    _preDelaySlider.setTextValueSuffix(" ms");
    _preDelaySlider.setValue(audioProcessor._DelayLineV[0]);

    addAndMakeVisible(_preDelaySlider);
        
    addAndMakeVisible (_preDelayLabel);
    _preDelayLabel.setText ("Pre Delay ", juce::dontSendNotification);
    _preDelayLabel.attachToComponent (&_preDelaySlider, true);
    
    setSize(600, 300);
}

ReverbGUIAudioProcessorEditor::~ReverbGUIAudioProcessorEditor()
{
}

//==============================================================================
void ReverbGUIAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::floralwhite);
    //SET VALUES OF VARIABLES TO CONTROL
    

}

void ReverbGUIAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    auto border=2;
    juce::Rectangle<int> area =getLocalBounds();
    
    juce::Rectangle<int> titleArea=area.removeFromTop((area.getHeight()/12));

    juce::Rectangle<int> dialArea=area.removeFromLeft((area.getWidth()/5));
    juce::Rectangle<int> lpLabelArea=dialArea.removeFromTop(dialArea.getHeight()/6);

    juce::Rectangle<int> toggleArea=dialArea.removeFromBottom(dialArea.getHeight()/6);
    
    
    juce::Rectangle<int> hpArea=area.removeFromLeft((area.getWidth()/4));
    
    juce::Rectangle<int> hpLabelArea=hpArea.removeFromTop(dialArea.getHeight()/6);
    juce::Rectangle<int> toggleHP=hpArea.removeFromBottom(hpArea.getHeight()/6);

    juce::Rectangle<int> blankArea=area.removeFromLeft((area.getWidth()/5));
    
    juce::Rectangle<int> wetArea=area.removeFromTop(area.getHeight()/4);

    juce::Rectangle<int> dryArea=area.removeFromTop(area.getHeight()/3);

    
    juce::Rectangle<int> predelayArea=area.removeFromTop(area.getHeight()/2);

    juce::Rectangle<int> roomSizeArea=area.removeFromTop(area.getHeight());

    _lpLabel.setBounds(lpLabelArea.reduced(border));
    _lpSlider.setBounds(dialArea.reduced(border));
    _lpButton.setBounds(toggleArea.reduced(_lpButton.getWidth()/2));
    
    _hpLabel.setBounds(hpLabelArea.reduced(border));
    _hpSlider.setBounds(hpArea.reduced(border));
    _hpButton.setBounds(toggleHP.reduced(border));

    _wetSlider.setBounds(wetArea.reduced(border));
    
    _drySlider.setBounds(dryArea.reduced(border));
    
    _preDelaySlider.setBounds(predelayArea.reduced(border));
    
    _roomSizeSlider.setBounds(roomSizeArea.reduced(border));
    

}

void ReverbGUIAudioProcessorEditor::sliderValueChanged(juce::Slider *slider){
    
    if(slider == &_lpSlider){
        //Cooking variables for Low Pass Filter
        audioProcessor._LP_f0=_lpSlider.getValue();
        audioProcessor._LP_w0= 2*  juce::MathConstants<float>::pi *
        audioProcessor._LP_f0/audioProcessor.Fs;
        audioProcessor._LP_alpha=sin(audioProcessor._LP_w0)/(2*audioProcessor.Q);
        //LPF
        audioProcessor._LP_a0=1 + audioProcessor._LP_alpha;
        audioProcessor._LP_a1 =-2* cos(audioProcessor._LP_w0);
        audioProcessor._LP_a2= 1- audioProcessor._LP_alpha;

        audioProcessor._LP_b0 =(1-cos(audioProcessor._LP_w0))/2;
        audioProcessor._LP_b1 =1-cos(audioProcessor._LP_w0);
        audioProcessor._LP_b2 =(1-cos(audioProcessor._LP_w0))/2;
        //Normalize
        audioProcessor._LP_b0 = audioProcessor._LP_b0 /audioProcessor._LP_a0;
        audioProcessor._LP_b1 = audioProcessor._LP_b1 /audioProcessor._LP_a0;
        audioProcessor._LP_b2 = audioProcessor._LP_b2 /audioProcessor._LP_a0;
        audioProcessor._LP_a1 = audioProcessor._LP_a1 /audioProcessor._LP_a0;
        audioProcessor._LP_a2 = audioProcessor._LP_a2 /audioProcessor._LP_a0;
        
    }
    if(slider == &_hpSlider){
        //Cooking variables for High Pass Filter
        audioProcessor._HP_f0=_hpSlider.getValue();
        audioProcessor._HP_w0= 2*  juce::MathConstants<float>::pi *
        audioProcessor._HP_f0/audioProcessor.Fs;
        audioProcessor._HP_alpha=sin(audioProcessor._HP_w0)/(2*audioProcessor.Q);
        //HPF
        
        audioProcessor._HP_a0=1 + audioProcessor._HP_alpha;
        audioProcessor._HP_a1 =-2* cos(audioProcessor._HP_w0);
        audioProcessor._HP_a2= 1- audioProcessor._HP_alpha;

        audioProcessor._HP_b0 =(1+cos(audioProcessor._HP_w0))/2;
        audioProcessor._HP_b1 =-(1+cos(audioProcessor._HP_w0));
        audioProcessor._HP_b2 =(1+cos(audioProcessor._HP_w0))/2;
        //Normalize
        audioProcessor._HP_b0 = audioProcessor._HP_b0 /audioProcessor._HP_a0;
        audioProcessor._HP_b1 = audioProcessor._HP_b1 /audioProcessor._HP_a0;
        audioProcessor._HP_b2 = audioProcessor._HP_b2 /audioProcessor._HP_a0;
        audioProcessor._HP_a1 = audioProcessor._HP_a1 /audioProcessor._HP_a0;
        audioProcessor._HP_a2 = audioProcessor._HP_a2 /audioProcessor._HP_a0;
        
    }
    if(slider == &_preDelaySlider){
        // Pre Delay in ms
        audioProcessor._DelayLineV.clear();
        audioProcessor._DelayLineV.push_back(_preDelaySlider.getValue());
        audioProcessor._DelayLineV.push_back(_preDelaySlider.getValue()+0.53);

        audioProcessor.D1_L.changeMDelay(audioProcessor._DelayLineV.front());
        audioProcessor.D1_R.changeMDelay(audioProcessor._DelayLineV.back());
        
    }
    if(slider == &_wetSlider){
        // Wet % 
        double temp=_wetSlider.getValue();
        
        audioProcessor._wet_Internal=temp/100;
        audioProcessor._wet=(int)temp;
        
        
    }
    if(slider == &_drySlider){
        double temp=_drySlider.getValue();

        audioProcessor._dry_Internal=temp/100;
        audioProcessor._dry=(int)temp;

    }
    if(slider == &_roomSizeSlider){

        audioProcessor._roomSize_Internal=_roomSizeSlider.getValue()*15;
        audioProcessor._LPCFV.clear();
        audioProcessor._NestedAPV.clear();

        audioProcessor._LPCFV.push_back(25.18972);
        audioProcessor._LPCFV.push_back(25.6972);
        audioProcessor._LPCFV.push_back(30.1972);
        audioProcessor._LPCFV.push_back(30.6972);
        audioProcessor._LPCFV.push_back(15.9892);
        audioProcessor._LPCFV.push_back(16.4893);
        audioProcessor._LPCFV.push_back(14.892);
        audioProcessor._LPCFV.push_back(15.3903);
        
        audioProcessor._NestedAPV.push_back(11.23);
        audioProcessor._NestedAPV.push_back(3.1);
        audioProcessor._NestedAPV.push_back(11.7546);
        audioProcessor._NestedAPV.push_back(3.6043);
        audioProcessor._NestedAPV.push_back(10.9560);
        audioProcessor._NestedAPV.push_back(3.456);
        audioProcessor._NestedAPV.push_back(11.4565);
        audioProcessor._NestedAPV.push_back(3.9053);
        audioProcessor._NestedAPV.push_back(7.6365);
        audioProcessor._NestedAPV.push_back(2.5342);
        audioProcessor._NestedAPV.push_back(8.165);
        audioProcessor._NestedAPV.push_back(2.9053);
        
        if (audioProcessor._roomSize_Internal>0.10)
        {
            //Only affecting variables if Room Size is bigger than a Threshold value
            audioProcessor._LPCFV[0]=audioProcessor._LPCFV[0]*audioProcessor._roomSize_Internal;
            audioProcessor._LPCFV[1]=audioProcessor._LPCFV[1]*audioProcessor._roomSize_Internal;
            audioProcessor._LPCFV[2]=audioProcessor._LPCFV[2]*audioProcessor._roomSize_Internal;
            audioProcessor._LPCFV[3]=audioProcessor._LPCFV[3]*audioProcessor._roomSize_Internal;
            audioProcessor._LPCFV[4]=audioProcessor._LPCFV[4]*audioProcessor._roomSize_Internal;
            audioProcessor._LPCFV[5]=audioProcessor._LPCFV[5]*audioProcessor._roomSize_Internal;
            audioProcessor._LPCFV[6]=audioProcessor._LPCFV[6]*audioProcessor._roomSize_Internal;
            audioProcessor._LPCFV[7]=audioProcessor._LPCFV[7]*audioProcessor._roomSize_Internal;
            audioProcessor._LPCFV[8]=audioProcessor._LPCFV[8]*audioProcessor._roomSize_Internal;

            audioProcessor._NestedAPV[0]=audioProcessor._NestedAPV[0]*audioProcessor._roomSize_Internal;
            audioProcessor._NestedAPV[1]=audioProcessor._NestedAPV[1]*audioProcessor._roomSize_Internal;
            audioProcessor._NestedAPV[2]=audioProcessor._NestedAPV[2]*audioProcessor._roomSize_Internal;
            audioProcessor._NestedAPV[3]=audioProcessor._NestedAPV[3]*audioProcessor._roomSize_Internal;
            audioProcessor._NestedAPV[4]=audioProcessor._NestedAPV[4]*audioProcessor._roomSize_Internal;
            audioProcessor._NestedAPV[5]=audioProcessor._NestedAPV[5]*audioProcessor._roomSize_Internal;
            audioProcessor._NestedAPV[6]=audioProcessor._NestedAPV[6]*audioProcessor._roomSize_Internal;
            audioProcessor._NestedAPV[7]=audioProcessor._NestedAPV[7]*audioProcessor._roomSize_Internal;
            audioProcessor._NestedAPV[8]=audioProcessor._NestedAPV[8]*audioProcessor._roomSize_Internal;
            audioProcessor._NestedAPV[9]=audioProcessor._NestedAPV[9]*audioProcessor._roomSize_Internal;
            audioProcessor._NestedAPV[10]=audioProcessor._NestedAPV[10]*audioProcessor._roomSize_Internal;
            audioProcessor._NestedAPV[11]=audioProcessor._NestedAPV[11]*audioProcessor._roomSize_Internal;
        }
        
        audioProcessor.lpfc1_L.changeMDelay(audioProcessor._LPCFV[0]);
        audioProcessor.lpfc1_R.changeMDelay(audioProcessor._LPCFV[1]);
        audioProcessor.lpfc2_L.changeMDelay(audioProcessor._LPCFV[2]);
        audioProcessor.lpfc2_R.changeMDelay(audioProcessor._LPCFV[3]);
        audioProcessor.lpfc3_L.changeMDelay(audioProcessor._LPCFV[4]);
        audioProcessor.lpfc3_R.changeMDelay(audioProcessor._LPCFV[5]);
        audioProcessor.lpfc4_L.changeMDelay(audioProcessor._LPCFV[6]);
        audioProcessor.lpfc4_R.changeMDelay(audioProcessor._LPCFV[7]);

        
        audioProcessor.NestedAP1_L.changeMDelay(audioProcessor._NestedAPV[0]);
        audioProcessor.NestedAP1_L.changeMDelayNested(audioProcessor._NestedAPV[1]);
        audioProcessor.NestedAP1_R.changeMDelay(audioProcessor._NestedAPV[2]);
        audioProcessor.NestedAP1_R.changeMDelayNested(audioProcessor._NestedAPV[3]);
        audioProcessor.NestedAP2_L.changeMDelay(audioProcessor._NestedAPV[4]);
        audioProcessor.NestedAP2_L.changeMDelayNested(audioProcessor._NestedAPV[5]);
        audioProcessor.NestedAP2_R.changeMDelay(audioProcessor._NestedAPV[6]);
        audioProcessor.NestedAP2_R.changeMDelayNested(audioProcessor._NestedAPV[7]);
        audioProcessor.NestedAP3_L.changeMDelay(audioProcessor._NestedAPV[8]);
        audioProcessor.NestedAP3_L.changeMDelayNested(audioProcessor._NestedAPV[9]);
        audioProcessor.NestedAP3_R.changeMDelay(audioProcessor._NestedAPV[10]);
        audioProcessor.NestedAP3_R.changeMDelayNested(audioProcessor._NestedAPV[11]);
    }
}

void ReverbGUIAudioProcessorEditor::buttonClicked(juce::Button *button){
    if(button == &_lpButton){
        audioProcessor._LP_flag= !audioProcessor._LP_flag;
    }
    if(button == &_hpButton){
        audioProcessor._HP_flag= !audioProcessor._HP_flag;
    }
}
