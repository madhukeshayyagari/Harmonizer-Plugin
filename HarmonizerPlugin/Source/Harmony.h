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
	Error_t CHarmony::ProcessGain();
	Error_t CHarmony::ProcessPan();
	Error_t CHarmony::ProcessPitchFactor();
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
	float   m_outputGainConverted;
	float   m_inputGainConverted;
	float   m_panLConverted;
	float	m_panRConverted;
	virtual ~CHarmony();
	CHarmony();



};

#endif // #if !defined(__Harmony_hdr__)

