#include <JuceHeader.h>
#include "TestSupport.h"

#include "shared/PhaseBuffer.h"
#include "shared/PhaseVocoder.h"
#include "SinusoidalShapedFilter/Source/SSF_FFTProcessor.h"
#include "shared/wavetable.h"

class SSFTests : public UnitTest
{
public:
    SSFTests() : UnitTest ("SinusoidalShapedFilter processor", "SpectralSuite") {}

    void runTest() override
    {
        testZeroTableSilences();
        testPhasePreservedAndWidthAttenuates();
    }

private:
    static constexpr int kSize = 512;
    static constexpr int kHalf = kSize / 2;

    std::shared_ptr<PhaseBuffer> makeBuffer() const
    {
        return std::make_shared<PhaseBuffer> (4, kSize);
    }

    static PolarVector makeInput()
    {
        PolarVector in (kHalf);
        for (int i = 0; i < kHalf; ++i)
            in[i] = Polar<FftDecimal> (2.f, 0.25f);
        return in;
    }

    void testZeroTableSilences()
    {
        beginTest ("an all-zero wavetable silences every bin");

        auto table = std::make_shared<Table<float>> (kHalf, 0);
        SSF_FFTProcessor filter (kSize, kHalf, 0, 48000, makeBuffer(), table);

        const auto in = makeInput();
        PolarVector out (kHalf);
        filter.spectral_process (in, out, kHalf);

        for (int i = 0; i < kHalf; ++i)
            expectWithinAbsoluteError (out[i].m_mag, 0.f, sstest::kEpsilon);
    }

    void testPhasePreservedAndWidthAttenuates()
    {
        beginTest ("phase is preserved and a larger width attenuates more");

        auto table = std::make_shared<Table<float>> (kHalf, 3); // half-sine, values in [0, 1]

        SSF_FFTProcessor narrow (kSize, kHalf, 0, 48000, makeBuffer(), table);
        narrow.setFrequency (0.f);
        narrow.setPhase (0.f);
        narrow.setWidth (0.f);

        SSF_FFTProcessor wide (kSize, kHalf, 0, 48000, makeBuffer(), table);
        wide.setFrequency (0.f);
        wide.setPhase (0.f);
        wide.setWidth (1.f);

        const auto in = makeInput();
        PolarVector narrowOut (kHalf), wideOut (kHalf);
        narrow.spectral_process (in, narrowOut, kHalf);
        wide.spectral_process (in, wideOut, kHalf);

        for (int i = 0; i < kHalf; ++i)
        {
            // phase always passes through
            expectWithinAbsoluteError (narrowOut[i].m_phase, in[i].m_phase, sstest::kEpsilon);
            expectWithinAbsoluteError (wideOut[i].m_phase, in[i].m_phase, sstest::kEpsilon);

            // more width can only reduce (or match) the shaped magnitude
            expectLessOrEqual (wideOut[i].m_mag, narrowOut[i].m_mag + sstest::kEpsilon);
        }

        // bin 0 sits at a wavetable zero crossing
        expectWithinAbsoluteError (narrowOut[0].m_mag, 0.f, sstest::kEpsilon);
    }
};

static SSFTests ssfTests;
