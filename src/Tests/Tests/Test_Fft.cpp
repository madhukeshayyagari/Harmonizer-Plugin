#include "MUSI6106Config.h"

#ifdef WITH_TESTS
#include <cassert>
#include <cstdio>

#include "UnitTest++.h"

#include "Synthesis.h"

//#include "Util.h"
#include "Vector.h"
#include "Fft.h"

SUITE(Fft)
{
    struct FftData
    {
        FftData() 
        {
            m_pfTime    = new float [m_iFftLength];
            m_pfFreq    = new float [m_iFftLength];
            m_pfReal    = new float [m_iFftLength];
            m_pfImag    = new float [m_iFftLength];
            m_pfTmp     = new float [m_iFftLength];

            CFft::createInstance(m_pCFftInstance);
            m_pCFftInstance->initInstance(m_iFftLength, 1, CFft::kWindowHann, CFft::kNoWindow);
        }

        ~FftData() 
        {
            m_pCFftInstance->resetInstance();
            CFft::destroyInstance(m_pCFftInstance);

            delete [] m_pfReal;
            delete [] m_pfImag;
            delete [] m_pfFreq;
            delete [] m_pfTime;
            delete [] m_pfTmp;
        }

        float *m_pfTime;
        CFft::complex_t *m_pfFreq;
        float *m_pfReal;
        float *m_pfImag;
        float *m_pfTmp;

        static const int m_iFftLength  = 1024;

        CFft *m_pCFftInstance;
    };

    TEST_FIXTURE(FftData, SimpleSine)
    {
        CSynthesis::generateSine(m_pfTime, 2.F, 1.F*m_iFftLength, m_iFftLength, 1.F, 0);
        
        m_pCFftInstance->doFft(m_pfFreq, m_pfTime);

        m_pCFftInstance->getPhase (m_pfTmp, m_pfFreq);
        CHECK_CLOSE(-M_PI_2, m_pfTmp[2], 1e-3);

        m_pCFftInstance->getMagnitude (m_pfTmp, m_pfFreq);
        for (int i=0; i < m_iFftLength/2+1; i++)
        {
            if (i!=2)
            {
                CHECK_CLOSE(0, m_pfTmp[i], 1e-3); 
            }
            else
            {
                CHECK_CLOSE(.5, m_pfTmp[i], 1e-3); 
            }
        }

        m_pCFftInstance->splitRealImag (m_pfReal, m_pfImag, m_pfFreq);

        for (int i=0; i < m_iFftLength/2+1; i++)
            CHECK_CLOSE(0, m_pfReal[i], 1e-3); 
        for (int i=0; i < m_iFftLength/2-1; i++)
        {
            if (i!=2)
            {
                CHECK_CLOSE(0, m_pfImag[i], 1e-3); 
            }
            else
            {
                CHECK_CLOSE(-.5, m_pfImag[i], 1e-3); 
            }
        }

        m_pCFftInstance->mergeRealImag (m_pfFreq, m_pfReal, m_pfImag);
        m_pCFftInstance->doInvFft(m_pfTmp, m_pfFreq);

        CHECK_ARRAY_CLOSE(m_pfTime, m_pfTmp, m_iFftLength, 1e-3);
    }
    TEST_FIXTURE(FftData, SimpleCos)
    {
        CSynthesis::generateSine(m_pfTime, 2.F, 1.F*m_iFftLength, m_iFftLength, 1.F, static_cast<float>(M_PI_2));

        m_pCFftInstance->doFft(m_pfFreq, m_pfTime);

        m_pCFftInstance->getPhase (m_pfTmp, m_pfFreq);
        CHECK_CLOSE(0, m_pfTmp[2], 1e-3);

        m_pCFftInstance->getMagnitude (m_pfTmp, m_pfFreq);
        for (int i=0; i < m_iFftLength/2+1; i++)
        {
            if (i!=2)
            {
                CHECK_CLOSE(0, m_pfTmp[i], 1e-3); 
            }
            else
            {
                CHECK_CLOSE(.5, m_pfTmp[i], 1e-3); 
            }
        }

        m_pCFftInstance->splitRealImag (m_pfReal, m_pfImag, m_pfFreq);

        for (int i=0; i < m_iFftLength/2; i++)
            CHECK_CLOSE(0, m_pfImag[i], 1e-3); 

        for (int i=0; i < m_iFftLength/2+1; i++)
        {
            if (i!=2)
            {
                CHECK_CLOSE(0, m_pfReal[i], 1e-3); 
            }
            else
            {
                CHECK_CLOSE(.5, m_pfReal[i], 1e-3); 
            }
        }
        m_pCFftInstance->mergeRealImag (m_pfFreq, m_pfReal, m_pfImag);
        m_pCFftInstance->doInvFft(m_pfTmp, m_pfFreq);

        CHECK_ARRAY_CLOSE(m_pfTime, m_pfTmp, m_iFftLength, 1e-3);
    }

    TEST_FIXTURE(FftData, Hann)
    {
        const int iDataLength = m_iFftLength>>3;

        m_pCFftInstance->resetInstance();
        m_pCFftInstance->initInstance(iDataLength, 8, CFft::kWindowHann, CFft::kPreWindow);
        CSynthesis::generateDc(m_pfTime, iDataLength, 1.F);

        m_pCFftInstance->doFft(m_pfFreq, m_pfTime);

        //reuse real-value buffer
        m_pCFftInstance->getMagnitude(m_pfReal, m_pfFreq);

        CHECK_CLOSE(64.F/m_iFftLength, m_pfReal[0], 1e-3); 
        CHECK_CLOSE(0, m_pfReal[16], 1e-3); 
        CHECK_CLOSE(1.7077F/m_iFftLength, m_pfReal[19], 1e-4); 
    }
    TEST_FIXTURE(FftData, Hamming)
    {
        const int iDataLength = m_iFftLength>>3;

        m_pCFftInstance->resetInstance();
        m_pCFftInstance->initInstance(iDataLength, 8, CFft::kWindowHamming, CFft::kPreWindow);
        CSynthesis::generateDc(m_pfTime, iDataLength, 1.F);

        m_pCFftInstance->doFft(m_pfFreq, m_pfTime);

        //m_pCFftInstance->getWindow(m_pfReal);

        //reuse real-value buffer
        m_pCFftInstance->getMagnitude(m_pfReal, m_pfFreq);

        CHECK_CLOSE(69.12F/m_iFftLength, m_pfReal[0], 1e-3); 
        CHECK_CLOSE(0, m_pfReal[16], 1e-3); 
        CHECK_CLOSE(0.5113/m_iFftLength, m_pfReal[36], 1e-4); 
    }

    TEST_FIXTURE(FftData, Inplace)
    {
        CSynthesis::generateNoise(m_pfTime, m_iFftLength, 1.F);

        // compute fft inplace and compare
        m_pCFftInstance->doFft(m_pfFreq, m_pfTime);
        CVectorFloat::copy(m_pfTmp, m_pfTime, m_iFftLength);
        m_pCFftInstance->doFft(m_pfTmp, m_pfTmp);
        CHECK_ARRAY_CLOSE(m_pfFreq, m_pfTmp, m_iFftLength, 1e-3);

        // get magnitude in-place and compare
        m_pCFftInstance->getMagnitude(m_pfReal, m_pfFreq);
        CVectorFloat::copy(m_pfTmp, reinterpret_cast<float*>(m_pfFreq), m_iFftLength);
        m_pCFftInstance->getMagnitude(m_pfTmp, m_pfTmp);
        CHECK_ARRAY_CLOSE(m_pfReal, m_pfTmp, m_pCFftInstance->getLength(CFft::kLengthMagnitude), 1e-3);

        // get phase in-place and compare
        m_pCFftInstance->getPhase(m_pfReal, m_pfFreq);
        CVectorFloat::copy(m_pfTmp, reinterpret_cast<float*>(m_pfFreq), m_iFftLength);
        m_pCFftInstance->getPhase(m_pfTmp, m_pfTmp);
        CHECK_ARRAY_CLOSE(m_pfReal, m_pfTmp, m_pCFftInstance->getLength(CFft::kLengthPhase), 1e-3);
    }

}

#endif //WITH_TESTS