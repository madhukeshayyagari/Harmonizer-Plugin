#include "Ppm.h"
#include <cmath>
#include <iostream>


using std::cout;
using std::endl;

Error_t CPpm::createInstance(CPpm *&pCPpm)
{
    pCPpm = new CPpm ();
    
    if (!pCPpm)
        return kUnknownError;
    
    return kNoError;
}

Error_t CPpm::destroyInstance(CPpm *&pCPpm)
{
    if (!pCPpm)
        return kUnknownError;
    
    delete pCPpm;
    pCPpm = 0;
    
    return kNoError;
}

CPpm::CPpm()
{
    reset();
    m_epsilon = 1.0f * exp(-5.0f);
    m_tempBuff = 0;
    m_vppmMax = 0;
    
}

CPpm::~CPpm()
{
    
    delete[] m_tempBuff;
    delete[] m_vppmMax;
    m_tempBuff = 0;
    m_vppmMax = 0;
}

Error_t CPpm::initInstance(float fSampleRateInHz, int iNumChannels)
{
    m_isInitialized = true;
    m_fSampleRate = fSampleRateInHz;
    m_iNumChannels = iNumChannels;
    
    //Set Alpha
    m_AlphaAT = 1 - exp(-2.2f / (m_fSampleRate*0.01));
    m_AlphaRT = 1 - exp(-2.2f / (m_fSampleRate*1.5));
    
    //Initialize
    m_tempBuff = new float[m_iNumChannels];
    m_vppmMax = new float[m_iNumChannels];
    for (int i = 0; i < m_iNumChannels; i++)
    {
        m_vppmMax[i] = 0;
        m_tempBuff[i] = 0;
    }
    
    return kNoError;
    
}

Error_t CPpm::process(float **ppfInputBuffer, float *ppfOutputBuff, int iNumberOfFrames)
{
    if (!m_isInitialized)
    {
        return kNotInitializedError;
    }
    else
    {
        for(int i=0; i< m_iNumChannels; i++) {
            m_vppmMax[i] = 0;
        }
        for (int i = 0; i < m_iNumChannels; i++)
        {
            for (int j = 0; j < iNumberOfFrames; j++) {
                if (m_tempBuff[i] > fabsf(ppfInputBuffer[i][j]))
                {
                    m_currentValue = (1 - m_AlphaRT)*m_tempBuff[i];
                }
                else
                {
                    m_currentValue = m_AlphaAT * fabsf(ppfInputBuffer[i][j]) + (1 - m_AlphaAT)*m_tempBuff[i];
                }
                
                m_tempBuff[i] = m_currentValue;
                
                if (m_currentValue > m_vppmMax[i]) {
                    m_vppmMax[i] = m_currentValue;
                }
                
            }
        }
        
        for (int i = 0; i < m_iNumChannels; i++)
        {
            if (m_vppmMax[i] < m_epsilon) {
                m_vppmMax[i] = m_epsilon;
            }
            
            ppfOutputBuff[i] = m_vppmMax[i];
        }
        return kNoError;
    }
}

Error_t CPpm::reset()
{
    m_currentValue = 0.0f;
    m_AlphaAT = 0.0f;
    m_AlphaRT = 0.0f;
    m_fSampleRate = 0.0f;
    m_iNumChannels = 0;
    
    
   
    m_tempBuff = 0;
    m_vppmMax = 0;
    return kNoError;
}


Error_t CPpm::setAlphaAT(float alphaATVal) {
    m_AlphaAT = alphaATVal;
    return kNoError;
}

Error_t CPpm::setAlphaRT(float alphaRTval) {
    m_AlphaRT = alphaRTval;
    return kNoError;
}
