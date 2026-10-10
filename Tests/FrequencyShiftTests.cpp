#include <JuceHeader.h>
#include "TestSupport.h"

#include "shared/PhaseBuffer.h"
#include "shared/PhaseVocoder.h"
#include "FrequencyShift/Source/FrequencyShiftFFTProcessor.h"

class FrequencyShiftTests : public UnitTest
{
public:
    FrequencyShiftTests() : UnitTest ("FrequencyShift processor", "SpectralSuite") {}

    void runTest() override
    {
        testUnityScalePassthrough();
        testScaleRemap();
        testPositiveBinShift();
        testNegativeBinShift();
    }

private:
    static constexpr int kSize = 1024;
    static constexpr int kHalf = kSize / 2;
    static constexpr int kRate = 8000;
    static constexpr int kShift = 100; // binWidth 0.128 -> binShift 12

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

    void testUnityScalePassthrough()
    {
        beginTest ("default scale of 1 passes the spectrum through unchanged");

        FrequencyShiftFFTProcessor shifter (kSize, kHalf, 0, kRate, makeBuffer());
        const auto in = makeInput();
        PolarVector out (kHalf);

        shifter.spectral_process (in, out, kHalf);

        for (int i = 0; i < kHalf; ++i)
            expectWithinAbsoluteError (out[i].m_mag, in[i].m_mag, sstest::kEpsilon);
    }

    void testScaleRemap()
    {
        beginTest ("scale of 2 maps bin i to bin 2i and leaves gaps silent");

        FrequencyShiftFFTProcessor shifter (kSize, kHalf, 0, kRate, makeBuffer());
        shifter.setScale (2.0f);

        const auto in = makeInput();
        PolarVector out (kHalf);
        shifter.spectral_process (in, out, kHalf);

        // every in-bin lands at double its index
        for (int i = 0; i < kHalf / 2; ++i)
            expectWithinAbsoluteError (out[i * 2].m_mag, in[i].m_mag, sstest::kEpsilon);

        // odd bins are not written by the mapping and stay silent
        expectWithinAbsoluteError (out[1].m_mag, 0.f, sstest::kEpsilon);
        expectWithinAbsoluteError (out[3].m_mag, 0.f, sstest::kEpsilon);
    }

    void testPositiveBinShift()
    {
        beginTest ("positive frequency shift moves bins upward and drops the tail");

        FrequencyShiftFFTProcessor shifter (kSize, kHalf, 0, kRate, makeBuffer());
        shifter.setShift (static_cast<float> (kShift));

        const auto in = makeInput();
        PolarVector out (kHalf);
        shifter.spectral_process (in, out, kHalf);

        // binShift = floor(100 * 1024 / 8000) = 12
        expectWithinAbsoluteError (out[12].m_mag, in[0].m_mag, sstest::kEpsilon);
        expectWithinAbsoluteError (out[13].m_mag, in[1].m_mag, sstest::kEpsilon);
        // input bins beyond halfSize - binShift are not shifted
        expectWithinAbsoluteError (out[kHalf - 1].m_mag, in[kHalf - 1 - 12].m_mag, sstest::kEpsilon);
    }

    void testNegativeBinShift()
    {
        beginTest ("negative frequency shift moves bins downward");

        FrequencyShiftFFTProcessor shifter (kSize, kHalf, 0, kRate, makeBuffer());
        shifter.setShift (static_cast<float> (-kShift));

        const auto in = makeInput();
        PolarVector out (kHalf);
        shifter.spectral_process (in, out, kHalf);

        expectWithinAbsoluteError (out[0].m_mag, in[12].m_mag, sstest::kEpsilon);
        expectWithinAbsoluteError (out[1].m_mag, in[13].m_mag, sstest::kEpsilon);
    }
};

static FrequencyShiftTests frequencyShiftTests;
