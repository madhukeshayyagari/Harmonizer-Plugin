// standard headers
#include <iostream>
#include <ctime>
#include <limits>
#include <math.h>
// project headers
#include "MUSI6106Config.h"

#include "ErrorDef.h"
#include "Harmony.h"

using std::cout;
using std::endl;

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
	return kNoError;
}

Error_t LowPass(float **input, float **output, int iNumberofFrames, int m_iNumChannels)
{
	float RC = 1 / 5000 * 2 * 3.14;
	float dt = 1 / 44100;
	float alpha = dt / (RC + dt);


	for (int i = 0; i < m_iNumChannels; i++)
	{
		output[i][0] = input[i][0];
		for (int j = 0; j < iNumberofFrames; j++)
		{
			output[i][j] - output[i][j - 1] + (alpha*(input[i][j] - output[i][j - 1]));
		}
	}
}



Error_t CHarmony::process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames, int block_size)
{
	if (!m_bIsInitialized)
		return kNotInitializedError;
	

	
	float fraction = m_PitchShiftFactor - floor(m_PitchShiftFactor);

	for (int i = 0; i < m_iNumChannels; i++)
	{
		
		
		float *temp = 0;
		temp = new float[iNumberOfFrames];
		for (int j = 0; j < ceil(iNumberOfFrames / m_PitchShiftFactor); j++)
		{
			float current = m_PitchShiftFactor * j;
			temp[j] = (ppfInputBuffer[i][int(ceil(current))] - ppfInputBuffer[i][int(floor(current))]) * fraction + ppfInputBuffer[i][int(floor(current))];

		}
		int count = 0;
		for (int j = 0; j < iNumberOfFrames; j++)
		{
			if (count == ceil(iNumberOfFrames / m_PitchShiftFactor))
				count = 0;
			if (temp[j] == 0 && j!=0)
				temp[j] = temp[j - 1];
			ppfOutputBuffer[i][j] = temp[count];
			count++;
		}
		/*for (int p = 0; p < iNumberOfFrames; p++)
		{
			cout << ppfOutputBuffer[i][p];
			cout << endl;
		}*/
		
			delete[] temp;
			temp = 0;

			
	}
	



//float LengthOfAudio = iNumberOfFrames*iCounter;

	
	
	//for (int i = 0; i < m_iNumChannels; i++)
	//{
	//	float *temp = 0;
	//	temp = new float[iNumberOfFrames];

	//	for (int c = 0; c < iNumberOfFrames; c++)
	//	{
	//		if (c % m_PitchShiftFactor == 0) {
	//			temp[c / m_PitchShiftFactor] = ppfInputBuffer[i][c];
	//		}
	//	}

	//	for (int c = 0; c < iNumberOfFrames/m_PitchShiftFactor; c++)
	//	{
	//		for (int j = 0; j < m_PitchShiftFactor; j++)
	//		{
 //               //ppfOutputBuffer[i][c] = temp[c];
	//			ppfOutputBuffer[i][(iNumberOfFrames / m_PitchShiftFactor) * j + c] = temp[c];
	//		}
	//	}

	//	delete[] temp;
	//	temp = 0;
	//}
	return kNoError;
}



