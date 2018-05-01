/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HarmonizerPluginAudioProcessorEditor::HarmonizerPluginAudioProcessorEditor (HarmonizerPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p),
	authorsLabel("", "Harmonizer by Henry Wang & Madhukesh Ayyagari"),
	groupLabel1("","Harmony Generator"),
	groupLabel2("", "Controls"),
	outputLabel("", "Output Gain"),
	inputLabel("", "Input Gain"),
	leftPanLabel("", "Pan Left"),
	rightPanLabel("", "Pan Right"),
	scaleLabel("", "Scale")
{
  
	addAndMakeVisible(&outputGainSlider);
	outputGainSlider.setSliderStyle(Slider::LinearHorizontal);
	outputGainSlider.setColour(Slider::thumbColourId, Colour(0xffffffff));
    outputGainSlider.addListener(this);
	outputGainSlider.setRange(-30.0, -9.0, 0.1);

	addAndMakeVisible(&inputGainSlider);
	inputGainSlider.setSliderStyle(Slider::LinearHorizontal);
	inputGainSlider.setColour(Slider::thumbColourId, Colour(0xffffffff));
	inputGainSlider.addListener(this);
	inputGainSlider.setRange(0.0, 6.0, 0.1);

	addAndMakeVisible(&LeftPanSlider);
	LeftPanSlider.setSliderStyle(Slider::LinearBarVertical);
	LeftPanSlider.setColour(Slider::thumbColourId, Colour(0xffffffff));
	LeftPanSlider.addListener(this);
	LeftPanSlider.setRange(0,100, 1);
    LeftPanSlider.setValue(50.0f);

	addAndMakeVisible(&RightPanSlider);
	RightPanSlider.setSliderStyle(Slider::LinearBarVertical);
	RightPanSlider.setColour(Slider::thumbColourId, Colour(0xffffffff));
	RightPanSlider.addListener(this);
    RightPanSlider.setRange(0,100, 1);
    RightPanSlider.setValue(50.0f);

	addAndMakeVisible(&PitchComboBox);
	PitchComboBox.setEditableText(false);
	PitchComboBox.setJustificationType(Justification::left);
	PitchComboBox.addItem("Third",1);
	PitchComboBox.addItem("Fifth", 2);
	PitchComboBox.addItem("Seventh", 3);
	PitchComboBox.addListener(this);

	addAndMakeVisible(&ScaleComboBox);
	ScaleComboBox.setEditableText(false);
	ScaleComboBox.setJustificationType(Justification::left);
	ScaleComboBox.addItem("Major", 1);
	ScaleComboBox.addItem("Minor", 2);
	ScaleComboBox.addListener(this);

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

	addAndMakeVisible(&leftPanLabel);
	leftPanLabel.setFont(Font(16.0f));
	leftPanLabel.setColour(Label::textColourId, Colours::white);

	addAndMakeVisible(&rightPanLabel);
	rightPanLabel.setFont(Font(16.0f));
	rightPanLabel.setColour(Label::textColourId, Colours::white);

	addAndMakeVisible(&scaleLabel);
	scaleLabel.setFont(Font(16.0f));
	scaleLabel.setColour(Label::textColourId, Colours::white);
	
	
	setSize (400, 400);


}

HarmonizerPluginAudioProcessorEditor::~HarmonizerPluginAudioProcessorEditor()
{
    deleteAndZero(groupComponent);
    deleteAndZero(groupComponent2);
}

//==============================================================================
void HarmonizerPluginAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll(Colour(0xff05122b));

}

void HarmonizerPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
	outputGainSlider.setBounds(220, 70, 150, 40);
	inputGainSlider.setBounds(220, 120, 150, 40);
	LeftPanSlider.setBounds(250, 180, 20, 150);
	RightPanSlider.setBounds(340, 180, 20, 150);
	PitchComboBox.setBounds(25, 160, 150, 30);
	ScaleComboBox.setBounds(25, 80, 150, 30);
	groupComponent->setBounds(15, 30, 180, 350);
	groupComponent2->setBounds(210, 30, 180, 350);
	authorsLabel.setBounds(17, 5, 600, 25);
	groupLabel1.setBounds(20, 375, 200, 25);
	groupLabel2.setBounds(260, 375, 200, 25);
	scaleLabel.setBounds(25, 50, 200, 25);
	outputLabel.setBounds(240, 55, 200, 25);
	inputLabel.setBounds(240, 105, 200, 25);
	leftPanLabel.setBounds(225, 330, 200, 25);
	rightPanLabel.setBounds(310, 330, 200, 25);

}

void HarmonizerPluginAudioProcessorEditor::sliderValueChanged(Slider * slider)
{
	
	if (slider == &outputGainSlider)
	{
		processor.setParameter(HarmonizerPluginAudioProcessor::koutputGain, (float)outputGainSlider.getValue());
	}

	if (slider == &inputGainSlider)
	{
		processor.setParameter(HarmonizerPluginAudioProcessor::kinputGain, (float)inputGainSlider.getValue());
	}

	if (slider == &LeftPanSlider)
	{
		processor.setParameter(HarmonizerPluginAudioProcessor::kpanLeft, (float)LeftPanSlider.getValue());
	}

	if (slider == &RightPanSlider)
	{
		processor.setParameter(HarmonizerPluginAudioProcessor::kpanRight, (float)RightPanSlider.getValue());
	}	
}	

void HarmonizerPluginAudioProcessorEditor::comboBoxChanged(ComboBox *comboBox)
{
	if (comboBox == &PitchComboBox) {
		processor.setParameter(HarmonizerPluginAudioProcessor::kpitchCombo, (float)PitchComboBox.getSelectedId());
	}
	if (comboBox == &ScaleComboBox) {
		processor.setParameter(HarmonizerPluginAudioProcessor::kscaleCombo, (float)ScaleComboBox.getSelectedId());
	}
}

