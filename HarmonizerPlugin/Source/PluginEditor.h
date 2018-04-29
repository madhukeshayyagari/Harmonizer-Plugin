/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class HarmonizerPluginAudioProcessorEditor  : public AudioProcessorEditor,
	public Slider::Listener,
	public ComboBox::Listener
{
public:
    HarmonizerPluginAudioProcessorEditor (HarmonizerPluginAudioProcessor&);
    ~HarmonizerPluginAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
	void sliderValueChanged(Slider* slider) override;
	void comboBoxChanged(ComboBox*);

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    HarmonizerPluginAudioProcessor& processor;
	Slider outputGainSlider, inputGainSlider, LeftPanSlider, RightPanSlider;
	ComboBox PitchComboBox, ScaleComboBox ;
	ScopedPointer<ResizableCornerComponent> resizer;
	ComponentBoundsConstrainer resizeLimits;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HarmonizerPluginAudioProcessorEditor)
};
