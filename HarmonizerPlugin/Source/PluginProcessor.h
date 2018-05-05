/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Harmony.h"
#include "ErrorDef.h"
#include "Ppm.h"



//==============================================================================
/**
*/
class HarmonizerPlugin1AudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    HarmonizerPlugin1AudioProcessor();
    ~HarmonizerPlugin1AudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    void setParameter(int paramIdx, float fNewValue) override;
	float getMaxPeakMeterValue(int channel);
    
    enum Parameters {
        koutputGain,
        kinputGain,
        kpan,
        kpitchCombo,
        kscaleCombo
    };
    
    enum Key {
        Third =  1,
        Fifth,
        Seventh
    };
    
    enum Scales {
        Major =1,
        Minor
    };



private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HarmonizerPlugin1AudioProcessor)
    
    CHarmony* pCHarmony = 0;
    float** ppfoldbuffer = 0;
    float m_pitchShiftFac = 1.0f;
    float const m_pitchShiftInit = 1.0f;

	CPpm *m_pCPpm = 0;
	float* m_fPpmValue = 0;
	float* m_fMaxPpmValue = 0;
	int m_iNumChannels = 0;
};
