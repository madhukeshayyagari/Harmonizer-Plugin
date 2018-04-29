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
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
	addAndMakeVisible(&outputGainSlider);
	outputGainSlider.setSliderStyle(Slider::LinearHorizontal);
	outputGainSlider.setColour(Slider::thumbColourId, Colour(0xffffffff));
    outputGainSlider.addListener(this);
	outputGainSlider.setRange(-30, 6, 0.1);

	addAndMakeVisible(&inputGainSlider);
	inputGainSlider.setSliderStyle(Slider::LinearHorizontal);
	inputGainSlider.setColour(Slider::thumbColourId, Colour(0xffffffff));
	inputGainSlider.addListener(this);
	inputGainSlider.setRange(-30, 6, 0.1);

	addAndMakeVisible(&LeftPanSlider);
	LeftPanSlider.setSliderStyle(Slider::LinearBarVertical);
	LeftPanSlider.setColour(Slider::thumbColourId, Colour(0xffffffff));
	LeftPanSlider.addListener(this);
	LeftPanSlider.setRange(-30, 6, 0.1);

	addAndMakeVisible(&RightPanSlider);
	RightPanSlider.setSliderStyle(Slider::LinearBarVertical);
	RightPanSlider.setColour(Slider::thumbColourId, Colour(0xffffffff));
	RightPanSlider.addListener(this);
	RightPanSlider.setRange(-30, 6, 0.1);

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
	
	addAndMakeVisible(resizer = new ResizableCornerComponent(this, &resizeLimits));
	resizeLimits.setSizeLimits(800, 600, 800, 600);
	setSize (800, 600);
}

HarmonizerPluginAudioProcessorEditor::~HarmonizerPluginAudioProcessorEditor()
{
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
	outputGainSlider.setBounds(400, 125, 150, 40);
	inputGainSlider.setBounds(250, 125, 150, 40);
	LeftPanSlider.setBounds(30, 250, 20, 150);
	RightPanSlider.setBounds(90, 250, 20, 150);
	PitchComboBox.setBounds(400, 245, 150, 30);
	ScaleComboBox.setBounds(180, 245, 150, 30);
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

	}
	if (comboBox == &ScaleComboBox) {

	}
}

