#include <JuceHeader.h>
#include "TestSupport.h"

#include "shared/PhaseBuffer.h"
#include "shared/PhaseVocoder.h"
#include "Morph/Source/MorphFFTProcessor.h"

class MorphTests : public UnitTest
{
public:
    MorphTests() : UnitTest ("Morph processor", "SpectralSuite") {}

    void runTest() override
    {
        testIdentityMapping();
        testReversedMapping();
        testMismatchedPointCountSilences();
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
        beginTest ("identity control points copy the spectrum through");

        Array<int> points;
        for (int i = 0; i < kHalf; ++i)
            points.add (i);
        Array<int>* pPoints = &points;

        MorphFFTProcessor morph (kSize, kHalf, 0, 48000, makeBuffer(), &pPoints);
        const auto in = makeInput();
        PolarVector out (kHalf);
        morph.spectral_process (in, out, kHalf);

        for (int i = 0; i < kHalf; ++i)
        {
            expectWithinAbsoluteError (out[i].m_mag, in[i].m_mag, sstest::kEpsilon);
            expectWithinAbsoluteError (out[i].m_phase, in[i].m_phase, sstest::kEpsilon);
        }
    }

    void testReversedMapping()
    {
        beginTest ("reversed control points move bin i to bin half-1-i");

        Array<int> points;
        for (int i = 0; i < kHalf; ++i)
            points.add (kHalf - 1 - i);
        Array<int>* pPoints = &points;

        MorphFFTProcessor morph (kSize, kHalf, 0, 48000, makeBuffer(), &pPoints);
        const auto in = makeInput();
        PolarVector out (kHalf);
        morph.spectral_process (in, out, kHalf);

        expectWithinAbsoluteError (out[kHalf - 1].m_mag, in[0].m_mag, sstest::kEpsilon);
        expectWithinAbsoluteError (out[0].m_mag, in[kHalf - 1].m_mag, sstest::kEpsilon);
    }

    void testMismatchedPointCountSilences()
    {
        beginTest ("a control point count that does not match the FFT half size silences output");

        Array<int> points;
        points.add (0);
        points.add (1);
        Array<int>* pPoints = &points;

        MorphFFTProcessor morph (kSize, kHalf, 0, 48000, makeBuffer(), &pPoints);
        const auto in = makeInput();
        PolarVector out (kHalf);
        morph.spectral_process (in, out, kHalf);

        for (int i = 0; i < kHalf; ++i)
            expectWithinAbsoluteError (out[i].m_mag, 0.f, sstest::kEpsilon);
    }
};

static MorphTests morphTests;
