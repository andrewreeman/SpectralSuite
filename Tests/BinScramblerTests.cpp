#include <JuceHeader.h>
#include "TestSupport.h"

#include "shared/PhaseBuffer.h"
#include "shared/PhaseVocoder.h"
#include "BinScrambler/Source/BinScramblerFFTProcessor.h"

class BinScramblerTests : public UnitTest
{
public:
    BinScramblerTests() : UnitTest ("BinScrambler processor", "SpectralSuite") {}

    void runTest() override
    {
        testIdentityMapping();
        testPhaseEndpoints();
        testInterpolationMidpoint();
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
            in[i] = Polar<FftDecimal> (static_cast<float> (i + 1), static_cast<float> (i) * 0.01f);
        return in;
    }

    void testIdentityMapping()
    {
        beginTest ("when A and B indices match, the spectrum is unchanged");

        std::vector<int> a (kHalf), b (kHalf);
        for (int i = 0; i < kHalf; ++i)
            a[static_cast<size_t> (i)] = b[static_cast<size_t> (i)] = i;

        BinScramblerFFTProcessor scrambler (kSize, kHalf, 0, 48000, makeBuffer(), &a, &b);
        scrambler.setPhase (0);
        scrambler.setMaxPhase (128);

        const auto in = makeInput();
        PolarVector out (kHalf);
        scrambler.spectral_process (in, out, kHalf);

        for (int i = 0; i < kHalf; ++i)
        {
            expectWithinAbsoluteError (out[i].m_mag, in[i].m_mag, sstest::kEpsilon);
            expectWithinAbsoluteError (out[i].m_phase, in[i].m_phase, sstest::kEpsilon);
        }
    }

    void testPhaseEndpoints()
    {
        beginTest ("at the start of a scramble the A index is used");

        std::vector<int> a (kHalf), b (kHalf);
        for (int i = 0; i < kHalf; ++i)
        {
            a[static_cast<size_t> (i)] = i;
            b[static_cast<size_t> (i)] = kHalf - 1 - i;
        }

        BinScramblerFFTProcessor scrambler (kSize, kHalf, 0, 48000, makeBuffer(), &a, &b);
        scrambler.setPhase (0);
        scrambler.setMaxPhase (128);

        const auto in = makeInput();
        PolarVector out (kHalf);
        scrambler.spectral_process (in, out, kHalf);

        expectWithinAbsoluteError (out[5].m_mag, in[5].m_mag, sstest::kEpsilon);
        expectWithinAbsoluteError (out[5].m_phase, in[5].m_phase, sstest::kEpsilon);
    }

    void testInterpolationMidpoint()
    {
        beginTest ("half way through a scramble interpolates A and B equally");

        std::vector<int> a (kHalf), b (kHalf);
        for (int i = 0; i < kHalf; ++i)
        {
            a[static_cast<size_t> (i)] = i;
            b[static_cast<size_t> (i)] = kHalf - 1 - i;
        }

        BinScramblerFFTProcessor scrambler (kSize, kHalf, 0, 48000, makeBuffer(), &a, &b);
        scrambler.setPhase (64); // 64 / 128 = 0.5
        scrambler.setMaxPhase (128);

        const auto in = makeInput();
        PolarVector out (kHalf);
        scrambler.spectral_process (in, out, kHalf);

        // i = 0 reads A = 0 (mag 1) and B = 255 (mag 256), so the midpoint is 128.5
        expectWithinAbsoluteError (out[0].m_mag, 128.5f, sstest::kEpsilon);
        // phases 0 and 2.55 interpolate to 1.275
        expectWithinAbsoluteError (out[0].m_phase, 1.275f, sstest::kEpsilon);
    }
};

static BinScramblerTests binScramblerTests;
