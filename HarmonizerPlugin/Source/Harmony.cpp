// standard headers
#include <iostream>
#include <ctime>
#include <limits>
#include <math.h>
#include <sstream>
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
	

	return kNoError;
}

Error_t CHarmony::reset()
{


	m_fSampleRateInHz = 0;
	m_iNumChannels = 0;
	m_bIsInitialized = false;
	m_bLfoIsInitialized = false;
	

	return kNoError;
}

Error_t CHarmony::setParam(float PitchShiftFactor) {

	m_PitchShiftFactor = PitchShiftFactor;
	return kNoError;
}

Error_t CHarmony::ProcessGain() {

    
    if (outputGainDB != -30.0f){
		m_outputGainConverted = pow(10.f, (outputGainDB / 20.f));
        std::cout<<m_outputGainConverted<<"\n";}
	else
		m_outputGainConverted = 0.0f;

	if (inputGainDB != -30.0f)
		m_inputGainConverted = pow(10.f, (inputGainDB / 20.f));
	else
		m_inputGainConverted = 0.0f;
	return kNoError;
}

Error_t CHarmony::ProcessPan()
{
	m_panLConverted = panLPer / 100.0f;
	m_panRConverted = panRPer / 100.0f;
	return kNoError;
	}

Error_t CHarmony::ProcessPitchFactor()
{
	if (pitchId == 1 && scaleId == 1) {
        m_PitchShiftFactor = pow(pow(2.0f, 1.0f/12.0f), 4.0f);
	}
	if (pitchId == 1 && scaleId == 2) {
        m_PitchShiftFactor = pow(pow(2.0f, 1.0f/12.0f), 3.0f);
	}
	if (pitchId == 2 && scaleId == 1) {
        m_PitchShiftFactor = pow(pow(2.0f, 1.0f/12.0f), 7.0f);
	}
	if (pitchId == 2 && scaleId == 2) {
        m_PitchShiftFactor = pow(pow(2.0f, 1.0f/12.0f), 6.0f);
	}
	if (pitchId == 3 && scaleId == 1) {
        m_PitchShiftFactor = pow(pow(2.0f, 1.0f/12.0f), 11.0f);
	}
	if (pitchId == 3 && scaleId == 2) {
        m_PitchShiftFactor = pow(pow(2.0f, 1.0f/12.0f), 10.0f);
	}

	return kNoError;
}

Error_t CHarmony::processHarmony(float **ppfPreviousBuffer,float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames)
{	
	//float fraction = m_PitchShiftFactor - floor(m_PitchShiftFactor);
    
    
    std::cout<<m_PitchShiftFactor<<"\n";
    
    m_tempBuff = new float* [m_iNumChannels];
    for (int i =0; i<m_iNumChannels;i++) {
        m_tempBuff[i] = new float[iNumberOfFrames];
    }
    
    for (int i=0;i<m_iNumChannels;i++) {
        for (int j=0; j<iNumberOfFrames;j++) {
            m_tempBuff[i][j] = 0;
        }
    }
    for (int i=0;i<m_iNumChannels;i++) {
        for (int j=0; j<iNumberOfFrames;j++) {
            m_tempBuff[i][j] = ppfInputBuffer[i][j];
        }
    }
    
    
    for (int i = 0; i < m_iNumChannels; i++) {
        
        cout << "channel" << endl;
        cout << i << endl;
        float *temp = 0;
        temp = new float[iNumberOfFrames*2];
        
        // populating the longer buffer
        int count = 0;
        for (int j = 0; j < iNumberOfFrames*2; j++) {
            if (count == iNumberOfFrames) {
                count = 0;
            }
            if (j < iNumberOfFrames) {
                temp[j] = ppfPreviousBuffer[i][j];
            } else {
                temp[j] = ppfInputBuffer[i][count];
            }
            count++;
    
        }
        
        // re-sample
        for (int c = 0; c < iNumberOfFrames; c++) {
            ppfOutputBuffer[i][c] = temp[(int)ceil(c*m_PitchShiftFactor)] + (temp[(int)ceil(c*m_PitchShiftFactor)] - temp[(int)floor(c*m_PitchShiftFactor)])*(m_PitchShiftFactor-1);
            
            std::cout<<ppfOutputBuffer[i][c]<<"\n";
            
            
        }
        
//        float *window = 0;
//        window = new float[iNumberOfFrames];
//
//        if (iNumberOfFrames % 2 != 0) {
//            for (int c = 0; c < iNumberOfFrames/2; c++) {
//                window[c] = window[iNumberOfFrames-c-1]=window[c] * (2.0*(c+1)/(iNumberOfFrames+1));
//            }
//            window[iNumberOfFrames/2] = 1.0;
//        } else {
//            for (int c = 0; c < iNumberOfFrames/2; c++) {
//                window[c] = window[iNumberOfFrames-c-1]=window[c] * (2.0*(c+1)/(iNumberOfFrames));
//            }
//        }
//
//        for (int c = 0; c < iNumberOfFrames; c++) {
//            ppfOutputBuffer[i][c] *= window[c];
//        }
//
//        delete[] window;
//        window = 0;
        
        
//        int count_tri = 1;
//        for (int c = 0; c < iNumberOfFrames; c++) {
//            if (c <= iNumberOfFrames/2) {
//                ppfOutputBuffer[i][c] = ppfOutputBuffer[i][c] * c * (1/(iNumberOfFrames/2));
//            } else {
//                ppfOutputBuffer[i][c] = ppfOutputBuffer[i][c] * (1 - count_tri * (1/(iNumberOfFrames/2)));
//                count_tri++;
//            }
//        }
//
		
    }
    
    
    
    
    
    
    
    
    
    
    
    
    
    
/*
    
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
    
    */

	for (int i = 0; i < m_iNumChannels; i++) {
		for (int j = 0; j < iNumberOfFrames; j++) {
            if (i == 0) {
                ppfOutputBuffer[i][j] =  ((m_tempBuff[i][j]*m_inputGainConverted)+(ppfOutputBuffer[i][j] * m_outputGainConverted))*(cos(pi*m_panLConverted/2));
                
//                cout<<m_panLConverted<<"\n";
             //cout<<m_outputGainConverted<<"\n";
			}
            if (i == 1) {
                ppfOutputBuffer[i][j] =  ((m_tempBuff[i][j]*m_inputGainConverted)+(ppfOutputBuffer[i][j] * m_outputGainConverted))*(sin(pi*m_panRConverted/2));
                
//                cout<<m_panRConverted<<"\n";
//                cout<<m_inputGainConverted<<"\n";
			}
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
    
    for (int i =0; i<m_iNumChannels; i++) {
        delete [] m_tempBuff[i];
    }
    delete [] m_tempBuff;
    m_tempBuff = 0;
	return kNoError;
}



