#include <JuceHeader.h>
#include "TestSupport.h"

#include "shared/PhaseBuffer.h"
#include "shared/PhaseVocoder.h"
#include "SpectralGate/Source/SpectralGateFFTProcessor.h"

class SpectralGateTests : public UnitTest
{
public:
    SpectralGateTests() : UnitTest ("SpectralGate processor", "SpectralSuite") {}

    void runTest() override
    {
        testGateBands();
        testTiltPreservesPhase();
    }

private:
    static constexpr int kSize = 512;
    static constexpr int kHalf = kSize / 2;
    static constexpr int kRate = 48000;

    std::shared_ptr<PhaseBuffer> makeBuffer() const
    {
        return std::make_shared<PhaseBuffer> (4, kSize);
    }

    void testGateBands()
    {
        beginTest ("magnitudes are split into strong / weak bands by cutoff and balance");

        // cutoff near 1 makes gate_high > gate_low, exposing the zero band.
        const float cutoff = 0.98f;
        SpectralGateFFTProcessor gate (kSize, kHalf, 0, kRate, makeBuffer());
        gate.setCutOff (cutoff);
        gate.setBalance (0.5f);

        const float gateHigh = std::pow (cutoff, 10.f);
        const float gateLow = cutoff * 0.6f;
        const float strongScale = std::pow (0.5f, 3.f);
        const float weakScale = std::pow (0.5f, 4.f);

        PolarVector in (kHalf), out (kHalf);
        for (int i = 0; i < kHalf; ++i)
            in[i] = Polar<FftDecimal> (0.7f, 0.25f);

        // strong bin
        in[1] = Polar<FftDecimal> (0.95f, 0.1f);
        // weak bin
        in[2] = Polar<FftDecimal> (0.1f, 0.2f);
        // zero band bin
        in[3] = Polar<FftDecimal> (0.7f, 0.3f);

        gate.spectral_process (in, out, kHalf);

        // bin 0 is copied straight through
        expectWithinAbsoluteError (out[0].m_mag, in[0].m_mag, sstest::kEpsilon);
        // 0.95 >= gateHigh -> strong
        expectWithinAbsoluteError (out[1].m_mag, 0.95f * strongScale, sstest::kEpsilon);
        // 0.1 < gateLow -> weak
        expectWithinAbsoluteError (out[2].m_mag, 0.1f * weakScale, sstest::kEpsilon);
        // gateLow <= 0.7 < gateHigh -> gated to silence
        expectWithinAbsoluteError (out[3].m_mag, 0.f, sstest::kEpsilon);

        // phases are always preserved by the gate
        expectWithinAbsoluteError (out[1].m_phase, 0.1f, sstest::kEpsilon);
        expectWithinAbsoluteError (out[2].m_phase, 0.2f, sstest::kEpsilon);
    }

    void testTiltPreservesPhase()
    {
        beginTest ("tilt changes the magnitude response but never the phase");

        SpectralGateFFTProcessor gate (kSize, kHalf, 0, kRate, makeBuffer());
        gate.setCutOff (0.98f);
        gate.setBalance (0.5f);
        gate.setTilt (1.0f);

        PolarVector in (kHalf), flatOut (kHalf), tiltOut (kHalf);
        for (int i = 0; i < kHalf; ++i)
            in[i] = Polar<FftDecimal> (0.7f, 0.4f);

        gate.setTiltEnabled (false);
        gate.spectral_process (in, flatOut, kHalf);

        gate.setTiltEnabled (true);
        gate.spectral_process (in, tiltOut, kHalf);

        bool magnitudeChanged = false;
        for (int i = 1; i < kHalf; ++i)
        {
            expectWithinAbsoluteError (tiltOut[i].m_phase, in[i].m_phase, sstest::kEpsilon);
            if (!sstest::almostEqual (tiltOut[i].m_mag, flatOut[i].m_mag))
                magnitudeChanged = true;
        }

        expect (magnitudeChanged, "enabling tilt should alter at least one bin's magnitude");
    }
};

static SpectralGateTests spectralGateTests;
