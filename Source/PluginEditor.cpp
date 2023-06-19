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
    _lpSlider.setValue(audioProcessor._reverbEngine._leftChannelLowPassFilter.getCutOffFrequency());

    addAndMakeVisible(_lpSlider);
    _lpButton.setButtonText("Active");
    _lpButton.setToggleState(audioProcessor._reverbEngine._LP_flag, juce::dontSendNotification);
    
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
    _hpSlider.setValue(audioProcessor._reverbEngine._rightChannelHighPassFilter.getCutOffFrequency());

    addAndMakeVisible(_hpSlider);
    _hpButton.setButtonText("Active");
    
    _hpButton.addListener(this);
    
    _hpButton.setToggleState(audioProcessor._reverbEngine._HP_flag, juce::dontSendNotification);

    addAndMakeVisible(_hpButton);
    
    addAndMakeVisible (_hpLabel);
    _hpLabel.setText ("High Pass ", juce::dontSendNotification);
    _hpLabel.setJustificationType(juce::Justification::centred);
    
    _wetSlider.setRange(0, 100,1);
    _wetSlider.addListener(this);
    addAndMakeVisible(_wetSlider);
    _wetSlider.setTextValueSuffix(" %");
    _wetSlider.setValue(audioProcessor._reverbEngine._wet);
    
    addAndMakeVisible (_wetLabel);
    _wetLabel.setText ("Wet % ", juce::dontSendNotification);
    _wetLabel.attachToComponent (&_wetSlider, true);
    
    
    _drySlider.setRange(0, 100,1);
    _drySlider.addListener(this);
    addAndMakeVisible(_drySlider);
    _drySlider.setTextValueSuffix(" %");
    _drySlider.setValue(audioProcessor._reverbEngine._dry);
    
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
    _preDelaySlider.setValue(audioProcessor._reverbEngine._DelayLineV[0]);

    addAndMakeVisible(_preDelaySlider);
        
    addAndMakeVisible (_preDelayLabel);
    _preDelayLabel.setText ("Pre Delay ", juce::dontSendNotification);
    _preDelayLabel.attachToComponent (&_preDelaySlider, true);
    
    setResizable(true, true);
    //setResizeLimits(300, 150, 900, 450);
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
    
    juce::Rectangle<int> hpLabelArea=hpArea.removeFromTop(hpArea.getHeight()/6);
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
        //Set CutOffFrequency and cookVariables
        audioProcessor._reverbEngine._leftChannelLowPassFilter.setCutOffFrequency(_lpSlider.getValue());
        audioProcessor._reverbEngine._leftChannelLowPassFilter.cookingVariables();
        
        audioProcessor._reverbEngine._rightChannelLowPassFilter.setCutOffFrequency(_lpSlider.getValue());
        audioProcessor._reverbEngine._rightChannelLowPassFilter.cookingVariables();
        
        
    }
    if(slider == &_hpSlider){
        //Cooking variables for High Pass Filter
        audioProcessor._reverbEngine._leftChannelHighPassFilter.setCutOffFrequency(_hpSlider.getValue());
        audioProcessor._reverbEngine._leftChannelHighPassFilter.cookingVariables();
        
        audioProcessor._reverbEngine._rightChannelHighPassFilter.setCutOffFrequency(_hpSlider.getValue());
        audioProcessor._reverbEngine._rightChannelHighPassFilter.cookingVariables();

        
    }
    if(slider == &_preDelaySlider){
        // Pre Delay in ms
        audioProcessor._reverbEngine._DelayLineV.clear();
        audioProcessor._reverbEngine._DelayLineV.push_back(_preDelaySlider.getValue());
        audioProcessor._reverbEngine._DelayLineV.push_back(_preDelaySlider.getValue()+0.53);

        audioProcessor._reverbEngine.D1_L.changeMDelay(audioProcessor._reverbEngine._DelayLineV.front());
        audioProcessor._reverbEngine.D1_R.changeMDelay(audioProcessor._reverbEngine._DelayLineV.back());
        
    }
    if(slider == &_wetSlider){
        // Wet % 
        double temp=_wetSlider.getValue();
        
        audioProcessor._reverbEngine._wet_Internal=temp/100;
        audioProcessor._reverbEngine._wet=(int)temp;
        
        
    }
    if(slider == &_drySlider){
        double temp=_drySlider.getValue();

        audioProcessor._reverbEngine._dry_Internal=temp/100;
        audioProcessor._reverbEngine._dry=(int)temp;

    }
    if(slider == &_roomSizeSlider){

        //audioProcessor._reverbEngine._roomSize_Internal=_roomSizeSlider.getValue()*15;
        
        audioProcessor._reverbEngine.setRoomSizeValue(_roomSizeSlider.getValue()*15);
        
        audioProcessor._reverbEngine.changeRoomSize();
        
        audioProcessor._reverbEngine.changeDelayValues();

    }
}

void ReverbGUIAudioProcessorEditor::buttonClicked(juce::Button *button){
    if(button == &_lpButton){
        audioProcessor._reverbEngine._LP_flag= !audioProcessor._reverbEngine._LP_flag;
    }
    if(button == &_hpButton){
        audioProcessor._reverbEngine._HP_flag= !audioProcessor._reverbEngine._HP_flag;
    }
}
