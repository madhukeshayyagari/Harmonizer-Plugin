/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "MeterComponent.h"


//==============================================================================
/**
*/
class HarmonizerPlugin1AudioProcessorEditor  : public AudioProcessorEditor,
public Slider::Listener,
public ComboBox::Listener,
public Timer
{
public:
    HarmonizerPlugin1AudioProcessorEditor (HarmonizerPlugin1AudioProcessor&);
    ~HarmonizerPlugin1AudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void sliderValueChanged(Slider* slider) override;
    void comboBoxChanged(ComboBox*) override;
	void timerCallback() override;
    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    HarmonizerPlugin1AudioProcessor& processor;
    Slider outputGainSlider, inputGainSlider, PanSlider;
    ComboBox PitchComboBox, ScaleComboBox ;
    
    
    
    GroupComponent* groupComponent;
    GroupComponent* groupComponent2;
    Label authorsLabel, groupLabel1,groupLabel2,outputLabel,inputLabel,leftPanLabel,rightPanLabel,scaleLabel;
	MeterComponent m_pPeakMeterCh1;
	MeterComponent m_pPeakMeterCh2;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HarmonizerPlugin1AudioProcessorEditor)
};


