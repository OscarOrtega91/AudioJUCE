/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "CustomStyle.h"

//==============================================================================
/**
*/
class ReverbGUIAudioProcessorEditor  : public juce::AudioProcessorEditor
                                       //public juce::Slider::Listener,
                                       //public juce::Button::Listener
{
public:
    ReverbGUIAudioProcessorEditor (ReverbGUIAudioProcessor&);
    ~ReverbGUIAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    //void sliderValueChanged( juce::Slider *slider) override;
    //void buttonClicked (juce::Button *button) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ReverbGUIAudioProcessor& audioProcessor;
    
    juce::OwnedArray<juce::SliderParameterAttachment> _sliderAttachment;
    juce::OwnedArray<juce::ButtonParameterAttachment> _buttonAttachment;

    juce::Slider _lpSlider;
    juce::Slider _hpSlider;
    juce::Slider _wetSlider;
    juce::Slider _drySlider;
    juce::Slider _preDelaySlider;
    juce::Slider _roomSizeSlider;

    juce::ToggleButton _lpButton;
    juce::ToggleButton _hpButton;
    juce::Label  _wetLabel;
    juce::Label  _dryLabel;
    juce::Label  _roomSizeLabel;
    juce::Label  _preDelayLabel;
    juce::Label  _lpLabel;
    juce::Label  _hpLabel;
    
    CustomStyle _customLooknFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReverbGUIAudioProcessorEditor)
};
