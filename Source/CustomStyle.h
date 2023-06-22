/*
  ==============================================================================

    CustomStyle.h
    Created: 22 Jun 2023 8:03:03am
    Author:  Oscar Ortega

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class CustomStyle : public juce::LookAndFeel_V4
{
public:
    CustomStyle();
    void drawRotarySlider (juce::Graphics &g , int x, int y, int width, int height,
                           float sliderPos, float rotaryStartAngle,
                           float rotaryEndAngle, juce::Slider &slider) override;

private:
    //Define Colours
    const juce::Colour outlineColour = juce::Colour::fromRGB(88, 88, 88);
    const juce::Colour fillColour = juce::Colour::fromRGB(253,165,97);

    //const juce::Colour fillColour = juce::Colour::fromRGB(255,248,5);
    const juce::Colour mainColour = juce::Colour::fromRGB(104,104,104);
    const juce::Colour brigthColour = juce::Colour::fromRGB(128,128,128);
    
    const juce::Colour thumbColourHorizontal = juce::Colour::fromRGB(255,149,67);
    const juce::Colour backgroundColourHorizontal = juce::Colour::fromRGB(138,138,138);

};
