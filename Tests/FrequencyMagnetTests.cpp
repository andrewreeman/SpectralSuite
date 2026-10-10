#include <JuceHeader.h>
#include "TestSupport.h"

#include "shared/PhaseBuffer.h"
#include "shared/PhaseVocoder.h"
#include "FrequencyMagnet/Source/FrequencyMagnetFFTProcessor.h"

class FrequencyMagnetTests : public UnitTest
{
public:
    FrequencyMagnetTests() : UnitTest ("FrequencyMagnet processor", "SpectralSuite") {}

    void runTest() override
    {
        testTargetBeyondBinsPassesThrough();
        testFullWidthIsIdentity();
    }

private:
    static constexpr int kSize = 1024;
    static constexpr int kHalf = kSize / 2;

    std::shared_ptr<PhaseBuffer> makeBuffer() const
    {
        return std::make_shared<PhaseBuffer> (4, kSize);
    }

    static PolarVector makeInput()
    {
        PolarVector in (kHalf);
        for (int i = 0; i < kHalf; ++i)
            in[i] = Polar<FftDecimal> (static_cast<float> (i + 1) * 0.1f, static_cast<float> (i) * 0.005f);
        return in;
    }

    void testTargetBeyondBinsPassesThrough()
    {
        beginTest ("a target above the bin count passes the spectrum through");

        FrequencyMagnetFFTProcessor magnet (kSize, kHalf, 0, 48000, makeBuffer());
        // target_bin = (28125 / 48000) * 1024 = 600 > 512
        magnet.setFrequency (28125.f);

        const auto in = makeInput();
        PolarVector out (kHalf);
        magnet.spectral_process (in, out, kHalf);

        for (int i = 0; i < kHalf; ++i)
        {
            expectWithinAbsoluteError (out[i].m_mag, in[i].m_mag, sstest::kEpsilon);
            expectWithinAbsoluteError (out[i].m_phase, in[i].m_phase, sstest::kEpsilon);
        }
    }

    void testFullWidthIsIdentity()
    {
        beginTest ("at width = 1 the spectrum is unchanged");

        FrequencyMagnetFFTProcessor magnet (kSize, kHalf, 0, 48000, makeBuffer());
        // target_bin = (12000 / 48000) * 1024 = 256
        magnet.setFrequency (12000.f);
        magnet.setWidth (1.f);
        magnet.setWidthBias (0.5f);

        const auto in = makeInput();
        PolarVector out (kHalf);
        magnet.spectral_process (in, out, kHalf);

        for (int i = 0; i < kHalf; ++i)
        {
            expectWithinAbsoluteError (out[i].m_mag, in[i].m_mag, 0.01f);
            expectWithinAbsoluteError (out[i].m_phase, in[i].m_phase, sstest::kEpsilon);
        }
    }
};

static FrequencyMagnetTests frequencyMagnetTests;
