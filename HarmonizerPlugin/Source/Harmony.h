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
	Error_t LowPass(float **input, float **output, int iNumberofFrames, int m_iNumChannels);


private:

	bool    m_bIsInitialized;   //!< internal bool to check whether the init function has been called
	bool    m_bRingBufferIsInitialized = false;
	bool    m_bLfoIsInitialized = false;
	float   m_fSampleRateInHz;
	int     m_iNumChannels;
	float   m_PitchShiftFactor;
	float   m_OverLapFactor;
	virtual ~CHarmony();
	CHarmony();



};

#endif // #if !defined(__Harmony_hdr__)

