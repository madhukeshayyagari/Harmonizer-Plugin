// standard headers
#include <iostream>
#include <ctime>
#include <limits>
#include <math.h>
// project header
#include "ErrorDef.h"
#include "Harmony.h"

# define pi           3.14159265358979323846  /* pi */


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

Error_t CHarmony::process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames)
{
	if (!m_bIsInitialized)
		return kNotInitializedError;
	
	float fraction = m_PitchShiftFactor - floor(m_PitchShiftFactor);
//    float factor = 1 / m_PitchShiftFactor -1; // used for interpolation
    
    float factor = (iNumberOfFrames-2) / (iNumberOfFrames * (1/m_PitchShiftFactor) - 2);
    
    // shifting down
    if (m_PitchShiftFactor < 1)
    {
        for (int i = 0; i < m_iNumChannels; i++)
        {
            float *temp = 0;
            float current = 1.0f;
            temp = new float[(int)ceil(iNumberOfFrames * (1/m_PitchShiftFactor))];
            for (int j = 0; j < (int)ceil(iNumberOfFrames * (1/m_PitchShiftFactor)); j++)
            {
                if (j == 0) temp[j] = ppfInputBuffer[i][j];
                else if (j == (int)ceil(iNumberOfFrames * (1/m_PitchShiftFactor))-1) {
                    temp[j] = ppfInputBuffer[i][iNumberOfFrames-1];
                }
                
                else {
                    current = current + factor;
                    temp[j] = (ppfInputBuffer[i][(int)ceil(current)] - ppfInputBuffer[i][(int)ceil(current)-1]) * factor;
                }
            }
            
            for (int c = 0; c < iNumberOfFrames; c++) {
                ppfOutputBuffer[i][c] = temp[c];
            }
            delete[] temp;
            temp = 0;
        }
    } else {

        for (int i = 0; i < m_iNumChannels; i++)
        {
            
            float *temp = 0;
            temp = new float[iNumberOfFrames];
            for (int j = 0; j < ceil(iNumberOfFrames / m_PitchShiftFactor); j++)
            {
                float current = m_PitchShiftFactor * j;
                temp[j] = (ppfInputBuffer[i][int(ceil(current))] - ppfInputBuffer[i][int(floor(current))]) * fraction + ppfInputBuffer[i][int(floor(current))];

            }
            // Adding Hann Window
//            int windowlen = ceil(iNumberOfFrames / m_PitchShiftFactor);
//            for (int n = 0; n < windowlen; n++)
//                temp[n] = 0.5f*(1.0f - cos(2 * pi*(float)n / (windowlen))) * temp[n];

            int count = 0;
            for (int j = 0; j < iNumberOfFrames; j++)
            {
                if (count == ceil(iNumberOfFrames / m_PitchShiftFactor))
                    count = 0;
                ppfOutputBuffer[i][j] = temp[count];
                count++;
            }
            
            delete[] temp;
            temp = 0;
        }
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



