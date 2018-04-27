
// standard headers
#include <limits>

// project headers
#include "MUSI6106Config.h"

#include "ErrorDef.h"
#include "Util.h"
#include "RingBuffer.h"

#include "CombFilterIf.h"
#include "CombFilter.h"


CCombFilterBase::CCombFilterBase( int iMaxDelayInFrames, int iNumChannels ) :
    m_ppCRingBuffer(0),
    m_iNumChannels(iNumChannels)
{

    assert(iMaxDelayInFrames > 0);
    assert(iNumChannels > 0);

    for (int i = 0; i < CCombFilterIf::kNumFilterParams; i++)
    {
        m_afParam[i]            = 0.F;
    }
    m_aafParamRange[CCombFilterIf::kParamGain][0]  = std::numeric_limits<float>::lowest();
    m_aafParamRange[CCombFilterIf::kParamGain][1]  = std::numeric_limits<float>::max();
    m_aafParamRange[CCombFilterIf::kParamDelay][0] = 0;
    m_aafParamRange[CCombFilterIf::kParamDelay][1] = static_cast<float>(iMaxDelayInFrames);

    m_ppCRingBuffer = new CRingBuffer<float>*[m_iNumChannels];
    for (int c = 0; c < m_iNumChannels; c++)
        m_ppCRingBuffer[c]  = new CRingBuffer<float>(iMaxDelayInFrames);
}

CCombFilterBase::~CCombFilterBase()
{
    if (m_ppCRingBuffer)
    {
        for (int c = 0; c < m_iNumChannels; c++)
            delete m_ppCRingBuffer[c];
    }
    delete [] m_ppCRingBuffer;
}

Error_t CCombFilterBase::resetInstance()
{
    for (int c = 0; c < m_iNumChannels; c++)
    {
        m_ppCRingBuffer[c]->reset ();
        m_ppCRingBuffer[c]->setWriteIdx(CUtil::float2int<int>(m_afParam[CCombFilterIf::kParamDelay]));
    }

    return kNoError;
}

Error_t CCombFilterBase::setParam( CCombFilterIf::FilterParam_t eParam, float fParamValue )
{
    if (!isInParamRange(eParam, fParamValue))
        return kFunctionInvalidArgsError;

    // special actions for special parameters
    if (eParam == CCombFilterIf::kParamDelay)
    {
        int iNumAdditionalTaps  = CUtil::float2int<int>(fParamValue - m_afParam[CCombFilterIf::kParamDelay]);
        if (iNumAdditionalTaps < 0)
        {
            for (int c = 0; c < m_iNumChannels; c++)
            {
                m_ppCRingBuffer[c]->setWriteIdx(CUtil::float2int<int>(fParamValue) + m_ppCRingBuffer[c]->getReadIdx());
            }
        }
        else
        {
            
            for (int c = 0; c < m_iNumChannels; c++)
            {
                for (int i = 0; i < iNumAdditionalTaps; i++)
                {
                    m_ppCRingBuffer[c]->putPostInc(0.F);
                }
            }
        }
    }

    m_afParam[eParam]   = fParamValue;

    return kNoError;
}

float CCombFilterBase::getParam( CCombFilterIf::FilterParam_t eParam ) const
{
    return m_afParam[eParam];
}

bool CCombFilterBase::isInParamRange( CCombFilterIf::FilterParam_t eParam, float fValue )
{
    if (fValue < m_aafParamRange[eParam][0] || fValue > m_aafParamRange[eParam][1])
    {
        return false;
    }
    else
    {
        return true;
    }
}

Error_t CCombFilterFir::process( float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames )
{
    for(int c = 0; c < m_iNumChannels; c++)
    {
        for (int i = 0; i < iNumberOfFrames; i++)
        {
            m_ppCRingBuffer[c]->putPostInc(ppfInputBuffer[c][i]);
            ppfOutputBuffer[c][i] = ppfInputBuffer[c][i] + m_afParam[CCombFilterIf::kParamGain] * m_ppCRingBuffer[c]->getPostInc();
        }
    }
    return kNoError;
}


CCombFilterIir::CCombFilterIir (int iMaxDelayInFrames, int iNumChannels) : CCombFilterBase(iMaxDelayInFrames, iNumChannels)
{
    // set limits
    m_aafParamRange[CCombFilterIf::kParamGain][0] = -1.F; 
    m_aafParamRange[CCombFilterIf::kParamGain][1] = 1.F;
}

Error_t CCombFilterIir::process( float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames )
{
    for(int c = 0; c < m_iNumChannels; c++)
    {
        for (int i = 0; i < iNumberOfFrames; i++)
        {
            ppfOutputBuffer[c][i] = ppfInputBuffer[c][i] + m_afParam[CCombFilterIf::kParamGain] * m_ppCRingBuffer[c]->getPostInc(); 
            m_ppCRingBuffer[c]->putPostInc(ppfOutputBuffer[c][i]);
        }
    }
    return kNoError;
}
