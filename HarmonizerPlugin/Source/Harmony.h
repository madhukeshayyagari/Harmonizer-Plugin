#if !defined(__Harmony_hdr__)
#define __Harmony_hdr__

#include "ErrorDef.h"


class CHarmony
{
public:

	

	static Error_t create(CHarmony*& pCHarmony);
	static Error_t destroy(CHarmony*& pCHarmony);
	Error_t init(float fSampleRateInHz, float fPitchShiftFactor, int iNumChannels);
	Error_t reset();
	Error_t setParam(float fPitchShiftFactor);
	Error_t process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames);
    Error_t ProcessGain();
    Error_t ProcessPan();
    Error_t ProcessPitchFactor();
	float   panLPer;
	float   panRPer;
	float   outputGainDB;
	float   inputGainDB;
	int		pitchId;
	int		scaleId;

private:

	bool    m_bIsInitialized;   //!< internal bool to check whether the init function has been called
	bool    m_bRingBufferIsInitialized = false;
	bool    m_bLfoIsInitialized = false;
	float   m_fSampleRateInHz;
	int     m_iNumChannels;
	float   m_PitchShiftFactor;
	float   m_OverLapFactor;
	float   m_outputGainConverted=1.0f;
	float   m_inputGainConverted=1.0f;
	float   m_panLConverted=0.5f;
	float	m_panRConverted=0.5f;
    
    float   ** m_tempBuff;
	virtual ~CHarmony();
	CHarmony();



};

#endif // #if !defined(__Harmony_hdr__)

