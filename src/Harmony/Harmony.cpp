// standard headers
#include <limits>
#include <math.h>
// project headers
#include "MUSI6106Config.h"

#include "ErrorDef.h"
#include "Util.h"
#include "Harmony.h"

CHarmony::CHarmony() :
	m_fSampleRateInHz(0)
{
	// this never hurts
	this->reset();
}

CHarmony::~CHarmony()
{
	this->reset();
}


Error_t CHarmony::create(CHarmony*& pCHarmony)
{
	pCHarmony = new CHarmony();

	if (!pCHarmony)
		return kUnknownError;


	return kNoError;
}

Error_t CHarmony::destroy(CHarmony*& pCHarmony)
{
	if (!pCHarmony)
		return kUnknownError;

	pCHarmony->reset();

	delete pCHarmony;
	pCHarmony = 0;

	return kNoError;
}

Error_t CHarmony::init(float fSampleRateInHz, float PitchShiftFactor, int iNumChannels) {
	reset();

	if (fSampleRateInHz <= 0 ||
		PitchShiftFactor <= 0 ||
		iNumChannels <= 0)
		return kFunctionInvalidArgsError;


	




	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//Initialize misc
	m_fSampleRateInHz = fSampleRateInHz;
	m_iNumChannels = iNumChannels;

	
	m_bIsInitialized = true;
	m_bLfoIsInitialized = true;
	m_bRingBufferIsInitialized = true;

	return kNoError;
}

Error_t CHarmony::reset()
{


	m_fSampleRateInHz = 0;
	m_iNumChannels = 0;
	m_bIsInitialized = false;
	m_bLfoIsInitialized = false;
	m_bRingBufferIsInitialized = false;

	return kNoError;
}

Error_t CHarmony::setParam(float PitchShiftFactor) {

	m_PitchShiftFactor = PitchShiftFactor;
}


Error_t CHarmony::process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames, int block_size)
{
	if (!m_bIsInitialized)
		return kNotInitializedError;

//float LengthOfAudio = iNumberOfFrames*iCounter;

	
	
	for (int i = 0; i < m_iNumChannels; i++)
	{
		float *temp = 0;
		temp = new float[iNumberOfFrames];

		for (int c = 0; c < iNumberOfFrames; c++)
		{
			if (c % 2 == 0) {
				temp[c / 2] = ppfInputBuffer[i][c];
			}
		}

		for (int c = 0; c < iNumberOfFrames/2; c++)
		{
			ppfOutputBuffer[i][c] = temp[c];
			ppfOutputBuffer[i][iNumberOfFrames / 2 + c] = temp[c];
		}

		delete[] temp;
		temp = 0;
	}
	return kNoError;
}



