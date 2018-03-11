#include "MUSI6106Config.h"

#ifdef WITH_TESTS
#include <cassert>
#include <cstdio>

#include "UnitTest++.h"

#include "Synthesis.h"

#include "RingBuffer.h"

SUITE(RingBuff)
{
    struct RingBuffer
    {
        RingBuffer() 
        {
            const float fSampleFreq  = 44100;

            m_pCRingBuffer  = new CRingBuffer<float> (m_iRingBuffLength);
            m_pfData        = new float [m_iDataLength];
            CSynthesis::generateSine(m_pfData, 20.F, fSampleFreq, m_iDataLength, .7F, static_cast<float>(M_PI_2));
        }

        ~RingBuffer() 
        {
            delete [] m_pfData;
            delete m_pCRingBuffer;
        }

 
        static const int m_iRingBuffLength;
        static const int m_iDataLength;
        static const int m_iDelay;

        CRingBuffer<float> *m_pCRingBuffer;

        float   *m_pfData;
    };

    const int RingBuffer::m_iRingBuffLength = 16;
    const int RingBuffer::m_iDataLength     = 55;
    const int RingBuffer::m_iDelay          = 9;

    TEST_FIXTURE(RingBuffer, RbDelay)
    {
        // put initial values into ring buffer
        for (int i = 0; i < m_iDelay; i++)
        {
            m_pCRingBuffer->putPostInc (m_pfData[i]);
        }

        for (int i = m_iDelay; i < m_iDataLength; i++)
        {
            CHECK_EQUAL(m_iDelay, m_pCRingBuffer->getNumValuesInBuffer());
            m_pCRingBuffer->getPostInc (); // just to increment - ignore the value
            m_pCRingBuffer->putPostInc (m_pfData[i]);
        }
    }

    TEST_FIXTURE(RingBuffer, RbValues)
    {
        int iDataBuffIdx    = 0;

        // put initial values into ring buffer
        for (int i = 0; i < m_iDelay; i++)
        {
            m_pCRingBuffer->putPostInc (m_pfData[i]);
        }

        for (int i = m_iDelay; i < m_iDataLength; i++, iDataBuffIdx++)
        {
            float fValue = m_pCRingBuffer->getPostInc ();
            CHECK_CLOSE(fValue, m_pfData[iDataBuffIdx], 1e-3F);
            m_pCRingBuffer->putPostInc (m_pfData[i]);
        }
    }

    TEST_FIXTURE(RingBuffer, RbReset)
    {
        // put initial values into ring buffer
        for (int i = 0; i < m_iDelay; i++)
        {
            m_pCRingBuffer->putPostInc (m_pfData[i]);
        }
        m_pCRingBuffer->reset ();
        CHECK_EQUAL(0, m_pCRingBuffer->getNumValuesInBuffer());

        for (int i = 0; i < m_iRingBuffLength; i++)
        {
            float fValue = m_pCRingBuffer->getPostInc ();
            CHECK_EQUAL (0.F, fValue);

            CHECK_EQUAL(m_iRingBuffLength-(i+1), m_pCRingBuffer->getNumValuesInBuffer());
        }
    }

    //===========================================
    //===========================================
    //===========================================
    TEST_FIXTURE(RingBuffer, RbPutGetRandom)
    {
        // generate random new index (can go negative)
        int newIdx = rand() - (RAND_MAX)/2;

        // generate random new sample value (-1.0 to 1.0)
        float newValue = static_cast <float>(rand()) / static_cast <float>(RAND_MAX) * 2.F - 1.F;

        // set write index and put new value
        m_pCRingBuffer->setWriteIdx(newIdx);
        m_pCRingBuffer->put(newValue);

        // test reading it
        m_pCRingBuffer->setReadIdx(newIdx);
        CHECK_EQUAL(newValue, m_pCRingBuffer->get());;

        // extra checking for the both index getters
        CHECK_EQUAL(m_pCRingBuffer->getWriteIdx(), m_pCRingBuffer->getReadIdx());
    }

    TEST_FIXTURE(RingBuffer, RbAccessors)
    {
        m_pCRingBuffer->setReadIdx(rand()%20);
        m_pCRingBuffer->setWriteIdx(m_pCRingBuffer->getReadIdx());
        CHECK_EQUAL(0, m_pCRingBuffer->getNumValuesInBuffer());

    }

    TEST_FIXTURE(RingBuffer, RbSetReadIdx)
    {
        // put initial values into ring buffer
        int i= 0;
        for (i = 0; i < m_iDelay; i++)
        {
            m_pCRingBuffer->putPostInc (m_pfData[i]);
        }
        m_pCRingBuffer->setReadIdx(i-1);
        CHECK_EQUAL(m_pfData[i-1] , m_pCRingBuffer->get());
    }

    TEST_FIXTURE(RingBuffer, RbGetSetIdx)
    {
        int test_Value = 10;

        m_pCRingBuffer->setReadIdx(test_Value);
        m_pCRingBuffer->setWriteIdx(test_Value);

        CHECK_EQUAL(test_Value, m_pCRingBuffer->getReadIdx());
        CHECK_EQUAL(test_Value, m_pCRingBuffer->getWriteIdx());

        m_pCRingBuffer->put(static_cast<float>(test_Value));
        m_pCRingBuffer->putPostInc(static_cast<float>(test_Value));

        CHECK_EQUAL(static_cast<float>(test_Value), m_pCRingBuffer->get());
        CHECK_EQUAL(static_cast<float>(test_Value), m_pCRingBuffer->getPostInc());
        CHECK_EQUAL(static_cast<float>(test_Value), m_pCRingBuffer->get(-1));
        CHECK_EQUAL(.0F, m_pCRingBuffer->get());

        //        Edge Cases
        m_pCRingBuffer->setReadIdx(RingBuffer::m_iRingBuffLength);
        m_pCRingBuffer->setWriteIdx(RingBuffer::m_iRingBuffLength);

        CHECK_EQUAL(0, m_pCRingBuffer->getReadIdx());
        CHECK_EQUAL(0, m_pCRingBuffer->getWriteIdx());

        m_pCRingBuffer->setReadIdx(RingBuffer::m_iRingBuffLength-1);
        m_pCRingBuffer->setWriteIdx(RingBuffer::m_iRingBuffLength-1);

        CHECK_EQUAL(RingBuffer::m_iRingBuffLength - 1, m_pCRingBuffer->getReadIdx());
        CHECK_EQUAL(RingBuffer::m_iRingBuffLength - 1, m_pCRingBuffer->getWriteIdx());

        m_pCRingBuffer->put(static_cast<float>(std::numeric_limits<int>::max()));
        m_pCRingBuffer->putPostInc(static_cast<float>(std::numeric_limits<int>::max()));

        CHECK_EQUAL(static_cast<float>(std::numeric_limits<int>::max()), m_pCRingBuffer->get());
        CHECK_EQUAL(static_cast<float>(std::numeric_limits<int>::max()), m_pCRingBuffer->getPostInc());        

        //        negative indices
        m_pCRingBuffer->setReadIdx(-1);
        m_pCRingBuffer->setWriteIdx(-1);
        CHECK_EQUAL(RingBuffer::m_iRingBuffLength-1, m_pCRingBuffer->getReadIdx());
        CHECK_EQUAL(RingBuffer::m_iRingBuffLength-1, m_pCRingBuffer->getWriteIdx());
        m_pCRingBuffer->setReadIdx(-RingBuffer::m_iRingBuffLength+1);
        m_pCRingBuffer->setWriteIdx(-RingBuffer::m_iRingBuffLength+1);
        CHECK_EQUAL(1, m_pCRingBuffer->getReadIdx());
        CHECK_EQUAL(1, m_pCRingBuffer->getWriteIdx());

        // extreme indices
        m_pCRingBuffer->setReadIdx(RingBuffer::m_iRingBuffLength * 5 - 1);
        m_pCRingBuffer->setWriteIdx(RingBuffer::m_iRingBuffLength * 5 - 1);
        CHECK_EQUAL(RingBuffer::m_iRingBuffLength - 1, m_pCRingBuffer->getReadIdx());
        CHECK_EQUAL(RingBuffer::m_iRingBuffLength - 1, m_pCRingBuffer->getWriteIdx());
        m_pCRingBuffer->setReadIdx(-RingBuffer::m_iRingBuffLength * 5 - 1);
        m_pCRingBuffer->setWriteIdx(-RingBuffer::m_iRingBuffLength * 5 - 1);
        CHECK_EQUAL(RingBuffer::m_iRingBuffLength - 1, m_pCRingBuffer->getReadIdx());
        CHECK_EQUAL(RingBuffer::m_iRingBuffLength - 1, m_pCRingBuffer->getWriteIdx());
    }

    // Simple test to check for overflow
    TEST_FIXTURE(RingBuffer, overflowTest)
    {

        for (int i = 0; i < 2*m_iRingBuffLength; i++)
        {
            m_pCRingBuffer->putPostInc (static_cast<float>(i));
        }

        for (int i=0;i< m_iRingBuffLength; i++)
        {
            CHECK_EQUAL(i + m_iRingBuffLength,m_pCRingBuffer->getPostInc());
        }
    }

    TEST_FIXTURE(RingBuffer, RbReadBlock)
    {
        for (int i = 0; i < 2*m_iRingBuffLength; i++)
        {
            m_pCRingBuffer->putPostInc (static_cast<float>(i));
        }
        m_pCRingBuffer->setReadIdx(5);
        m_pCRingBuffer->getPostInc(m_pfData, m_iRingBuffLength);

        for (int i=0;i< m_iRingBuffLength; i++)
        {
            CHECK_EQUAL((i+5)%m_iRingBuffLength + m_iRingBuffLength,m_pfData[i]);
        }
    }

    TEST_FIXTURE(RingBuffer, RbWriteBlock)
    {
        m_pCRingBuffer->putPostInc (m_pfData, 11);
        m_pCRingBuffer->putPostInc (&m_pfData[11], m_iRingBuffLength);
        m_pCRingBuffer->setReadIdx(11);

        for (int i = 11; i < 11+m_iRingBuffLength; i++)
        {
            CHECK_EQUAL(m_pfData[i], m_pCRingBuffer->getPostInc());
        }
    }

    TEST_FIXTURE(RingBuffer, RbFracDelay)
    {
        for (int i = 0; i < m_iRingBuffLength; i++)
            m_pCRingBuffer->putPostInc (1.F*i);

        float fValue    = m_pCRingBuffer->get(.7F);
        CHECK_CLOSE(.7F, fValue, 1e-4);

        fValue = m_pCRingBuffer->get(-.5F);
        CHECK_CLOSE(7.5, fValue, 1e-4);

        fValue = m_pCRingBuffer->get(-1.8F);
        CHECK_CLOSE(14.2F, fValue, 1e-4);

        m_pCRingBuffer->setReadIdx(1);
        fValue          = m_pCRingBuffer->get(-m_iRingBuffLength+1.F);
        CHECK_CLOSE(2.F, fValue, 1e-4);
    }
}

#endif //WITH_TESTS