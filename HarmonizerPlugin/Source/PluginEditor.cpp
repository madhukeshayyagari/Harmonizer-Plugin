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

	addAndMakeVisible(&transposeSlider);
	transposeSlider.setSliderStyle(Slider::Rotary);
	transposeSlider.addListener(this);
	transposeSlider.setRange(-12, 12, 0.1);
	transposeSlider.setColour(Slider::rotarySliderFillColourId, Colour(0xfbffffff));
	transposeSlider.setColour(Slider::rotarySliderOutlineColourId, Colour(0xff3c3c54));

	addAndMakeVisible(&inputGainSlider);
	inputGainSlider.setSliderStyle(Slider::LinearHorizontal);
	inputGainSlider.setColour(Slider::thumbColourId, Colour(0xffffffff));
	inputGainSlider.addListener(this);
	inputGainSlider.setRange(-30, 6, 0.1);

	addAndMakeVisible(&LeftSlider);
	LeftSlider.setSliderStyle(Slider::LinearBarVertical);
	LeftSlider.setColour(Slider::thumbColourId, Colour(0xffffffff));
	LeftSlider.addListener(this);
	LeftSlider.setRange(-30, 6, 0.1);

	addAndMakeVisible(&RightSlider);
	RightSlider.setSliderStyle(Slider::LinearBarVertical);
	RightSlider.setColour(Slider::thumbColourId, Colour(0xffffffff));
	RightSlider.addListener(this);
	RightSlider.setRange(-30, 6, 0.1);

	addAndMakeVisible(&PitchComboBox);
	PitchComboBox.setEditableText(false);
	PitchComboBox.setJustificationType(Justification::left);
	PitchComboBox.addItem("1",1);
	PitchComboBox.addItem("2", 2);
	PitchComboBox.addItem("3", 3);
	PitchComboBox.addItem("4", 4);
	PitchComboBox.addItem("5", 5);
	PitchComboBox.addItem("6", 6);
	PitchComboBox.addItem("7", 7);
	PitchComboBox.addItem("8", 8);
	PitchComboBox.addItem("9", 9);
	PitchComboBox.addItem("10", 10);
	PitchComboBox.addItem("11", 11);
	PitchComboBox.addItem("12", 12);
	PitchComboBox.addListener(this);
	
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
	transposeSlider.setBounds(30, 125, 150, 40);
	LeftSlider.setBounds(30, 250, 20, 150);
	RightSlider.setBounds(90, 250, 20, 150);
	PitchComboBox.setBounds(180, 245, 150, 30);
}

void HarmonizerPluginAudioProcessorEditor::sliderValueChanged(Slider * slider)
{
	//if (slider == &m_sliderModFreq)
	//{

	//	//processor.fModFreq = m_sliderModFreq.getValue();
	//	processor.setParameter(0, m_sliderModFreq.getValue());


	//}
	//else if (slider == &m_sliderModWidth)
	//{
	//	//processor.fModWidth = m_sliderModWidth.getValue();
	//	processor.setParameter(1, m_sliderModWidth.getValue());

	//}
}

void HarmonizerPluginAudioProcessorEditor::initializeSlider(Slider & slider, float Min, float Max, float Interval, float x, float y, float width, float height, const String & displayText, float fdefault)
{
	slider.setSliderStyle(Slider::LinearBarVertical);
	slider.setRange(Min, Max, Interval);
	slider.setTextValueSuffix(displayText);
	slider.setValue(fdefault);
	slider.setBounds(x, y, width, height);
	slider.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
	slider.addListener(this);
	addAndMakeVisible(&slider);
}

void HarmonizerPluginAudioProcessorEditor::comboBoxChanged(ComboBox *)
{
}

