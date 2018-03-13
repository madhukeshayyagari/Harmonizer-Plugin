#if !defined(__Harmony_hdr__)
#define __Harmony_hdr__

#include "ErrorDef.h"


class CHarmony
{
public:

	

	static Error_t create(CHarmony*& pCHarmony);
	static Error_t destroy(CHarmony*& pCHarmony);
	Error_t init(float fSampleRateInHz, float PitchShiftFactor, int iNumChannels);
	Error_t reset();
	Error_t setParam(float PitchShiftFactor);
	Error_t process(float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames, int block_size);



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

