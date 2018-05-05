/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
HarmonizerPlugin1AudioProcessorEditor::HarmonizerPlugin1AudioProcessorEditor (HarmonizerPlugin1AudioProcessor& p)
: AudioProcessorEditor (&p), processor (p),
authorsLabel("", "Harmonizer by Madhukesh Ayyagari & Henry Wang"),
groupLabel1("","Harmony Generator"),
groupLabel2("", "Controls"),
outputLabel("", "Output Gain"),
inputLabel("", "Input Gain"),
rightPanLabel("", "Pan"),
scaleLabel("", "Select Harmony")
{
	startTimerHz(100);
    
    addAndMakeVisible(&outputGainSlider);
    outputGainSlider.setSliderStyle(Slider::LinearHorizontal);
    outputGainSlider.setColour(Slider::thumbColourId, Colour(0xff05124b));
    outputGainSlider.addListener(this);
    outputGainSlider.setRange(-30.0, -9.0, 0.1);
    outputGainSlider.setValue(-9.0);
    
    
    addAndMakeVisible(&inputGainSlider);
    inputGainSlider.setSliderStyle(Slider::LinearHorizontal);
    inputGainSlider.setColour(Slider::thumbColourId, Colour(0xff05124b));
    inputGainSlider.addListener(this);
    inputGainSlider.setRange(0.0, 6.0, 0.1);
    inputGainSlider.setValue(0.2);
       
    addAndMakeVisible(&PanSlider);
    PanSlider.setSliderStyle(Slider::LinearBarVertical);
    PanSlider.setColour(Slider::trackColourId, Colour(0xff05124b));
    PanSlider.addListener(this);
    PanSlider.setRange(0,100, 1);
    PanSlider.setValue(50.0f);
	PanSlider.setTextBoxStyle(Slider:: TextBoxAbove, false, 160, 30);
	
    
    addAndMakeVisible(&PitchComboBox);
    PitchComboBox.setEditableText(false);
    PitchComboBox.setJustificationType(Justification::left);
    PitchComboBox.addItem("Third",1);
    PitchComboBox.addItem("Fifth", 2);
    PitchComboBox.addItem("Seventh", 3);
    PitchComboBox.addListener(this);
	PitchComboBox.setColour(ComboBox::backgroundColourId, Colour(0xff05123b));
    
    addAndMakeVisible(&ScaleComboBox);
    ScaleComboBox.setEditableText(false);
    ScaleComboBox.setJustificationType(Justification::left);
    ScaleComboBox.addItem("Major", 1);
    ScaleComboBox.addItem("Minor", 2);
    ScaleComboBox.addListener(this);
	ScaleComboBox.setColour(ComboBox::backgroundColourId, Colour(0xff05123b));

	
    
    addAndMakeVisible(groupComponent = new GroupComponent(String::empty,
                                                          String::empty));
    groupComponent->setColour(GroupComponent::outlineColourId, Colours::dimgrey);
    groupComponent->setColour(GroupComponent::textColourId, Colours::white);
    
    addAndMakeVisible(groupComponent2 = new GroupComponent(String::empty,
                                                           String::empty));
    groupComponent->setColour(GroupComponent::outlineColourId, Colours::dimgrey);
    groupComponent->setColour(GroupComponent::textColourId, Colours::white);
    
    addAndMakeVisible(&authorsLabel);
    authorsLabel.setFont(Font(17.0f));
    authorsLabel.setColour(Label::textColourId, Colours::white);
    
    addAndMakeVisible(&groupLabel1);
    groupLabel1.setFont(Font(19.0f));
    groupLabel1.setColour(Label::textColourId, Colours::white);
    
    addAndMakeVisible(&groupLabel2);
    groupLabel2.setFont(Font(19.0f));
    groupLabel2.setColour(Label::textColourId, Colours::white);
    
    addAndMakeVisible(&outputLabel);
    outputLabel.setFont(Font(16.0f));
    outputLabel.setColour(Label::textColourId, Colours::white);
    
    addAndMakeVisible(&inputLabel);
    inputLabel.setFont(Font(16.0f));
    inputLabel.setColour(Label::textColourId, Colours::white);
      
    addAndMakeVisible(&rightPanLabel);
    rightPanLabel.setFont(Font(16.0f));
    rightPanLabel.setColour(Label::textColourId, Colours::white);
    
    addAndMakeVisible(&scaleLabel);
    scaleLabel.setFont(Font(16.0f));
    scaleLabel.setColour(Label::textColourId, Colours::white);

	addAndMakeVisible(m_pPeakMeterCh1);
	addAndMakeVisible(m_pPeakMeterCh2);

	
    
    
    setSize (400, 400);
    

}

HarmonizerPlugin1AudioProcessorEditor::~HarmonizerPlugin1AudioProcessorEditor()
{
    deleteAndZero(groupComponent);
    deleteAndZero(groupComponent2);
}

//==============================================================================
void HarmonizerPlugin1AudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(Colour(0xff05122b));

	g.setColour(Colours::white);
	g.setFont(15.0f);
	g.drawFittedText("dB", 45, 330, 20, 20, Justification::left, 1);
	g.drawFittedText("dB", 140, 330, 20, 20, Justification::left, 1);
	
}

void HarmonizerPlugin1AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    outputGainSlider.setBounds(220, 70, 150, 40);
    inputGainSlider.setBounds(220, 120, 150, 40);
    PanSlider.setBounds(275, 180, 20, 150);
    PitchComboBox.setBounds(25, 130, 150, 30);
    ScaleComboBox.setBounds(25, 80, 150, 30);
    groupComponent->setBounds(15, 30, 180, 350);
    groupComponent2->setBounds(210, 30, 180, 350);
    authorsLabel.setBounds(17, 5, 600, 25);
    groupLabel1.setBounds(20, 375, 200, 25);
    groupLabel2.setBounds(260, 375, 200, 25);
    scaleLabel.setBounds(25, 50, 200, 25);
    outputLabel.setBounds(240, 55, 200, 25);
    inputLabel.setBounds(240, 105, 200, 25);
    rightPanLabel.setBounds(270, 330, 200, 25);
	m_pPeakMeterCh1.setBounds(45, 180, 20, 150);
	m_pPeakMeterCh2.setBounds(140, 180, 20, 150);
}
void HarmonizerPlugin1AudioProcessorEditor::sliderValueChanged(Slider * slider)
{
    
    if (slider == &outputGainSlider)
    {
        processor.setParameter(HarmonizerPlugin1AudioProcessor::koutputGain, (float)outputGainSlider.getValue());
    }
    
    if (slider == &inputGainSlider)
    {
        processor.setParameter(HarmonizerPlugin1AudioProcessor::kinputGain, (float)inputGainSlider.getValue());
    }
    
    if (slider == &PanSlider)
    {
        processor.setParameter(HarmonizerPlugin1AudioProcessor::kpan, (float)PanSlider.getValue());
    }
}

void HarmonizerPlugin1AudioProcessorEditor::comboBoxChanged(ComboBox *comboBox)
{
    if (comboBox == &PitchComboBox) {
        processor.setParameter(HarmonizerPlugin1AudioProcessor::kpitchCombo, (float)PitchComboBox.getSelectedId());
    }
    if (comboBox == &ScaleComboBox) {
        processor.setParameter(HarmonizerPlugin1AudioProcessor::kscaleCombo, (float)ScaleComboBox.getSelectedId());
    }
}

void HarmonizerPlugin1AudioProcessorEditor::timerCallback()
{
	m_pPeakMeterCh1.setPMValue(processor.getMaxPeakMeterValue(0));
	m_pPeakMeterCh2.setPMValue(processor.getMaxPeakMeterValue(1));
}

