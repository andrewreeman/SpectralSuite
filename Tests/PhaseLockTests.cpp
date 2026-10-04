#include <JuceHeader.h>
#include "TestSupport.h"

#include "shared/PhaseBuffer.h"
#include "shared/PhaseVocoder.h"
#include "PhaseLock/Source/LockState.h"
#include "PhaseLock/Source/TransitionState.h"
#include "PhaseLock/Source/PhaseLockFFTProcessor.h"

class PhaseLockTests : public UnitTest
{
public:
    PhaseLockTests() : UnitTest ("PhaseLock processor", "SpectralSuite") {}

    void runTest() override
    {
        testLockStateTransitions();
        testTransitionStateRamp();
        testLockCapturesAndHoldsSpectrum();
    }

private:
    void testLockStateTransitions()
    {
        beginTest ("LockState moves off -> change -> on and resets sanely");
        LockState state;
        expect (state.isOff());
        expect (!state.isOn());

        state.beginTransitionToOn();
        expect (state.shouldTransitionToOn());
        expect (!state.isOn());

        state.completeTransitionToOn();
        expect (state.isOn());

        state.off();
        expect (state.isOff());

        // reset while ON requests another transition, while OFF stays off
        state.completeTransitionToOn();
        state.reset();
        expect (state.shouldTransitionToOn());
    }

    void testTransitionStateRamp()
    {
        beginTest ("TransitionState ramps from 0 to 1 and back down");

        TransitionState transition (100, 10); // 100 Hz * 1 s = 100, step 10
        expect (transition.isOff());

        // ramp is a no-op until explicitly started
        expectWithinAbsoluteError (transition.next(), 0.f, sstest::kEpsilon);
        expect (transition.isOff());

        transition.start();
        expect (transition.shouldStartTransition());
        expectWithinAbsoluteError (transition.next(), 0.f, sstest::kEpsilon);

        expect (transition.isTransitioningIn());
        expectWithinAbsoluteError (transition.next(), 0.1f, sstest::kEpsilon);
        expectWithinAbsoluteError (transition.next(), 0.2f, sstest::kEpsilon);

        // run the ramp to completion
        for (int i = 0; i < 20; ++i)
            transition.next();

        // fully in = 1
        expectWithinAbsoluteError (transition.next(), 1.f, sstest::kEpsilon);

        // stopping begins a fade back to silence
        transition.stop();
        expect (transition.isTransitioningOut());
        for (int i = 0; i < 30; ++i)
            transition.next();
        expect (transition.isOff());
    }

    void testLockCapturesAndHoldsSpectrum()
    {
        beginTest ("locking mag and phase freezes the captured spectrum");

        constexpr int size = 512;
        constexpr int half = size / 2;
        constexpr int bins = 4;

        auto buffer = std::make_shared<PhaseBuffer> (4, size);
        PhaseLockFFTProcessor processor (size, half, 0, 48000, buffer);
        processor.setMagTrack (0.f);

        PolarVector first (bins), second (bins), out (bins);
        for (int i = 0; i < bins; ++i)
        {
            first[i] = Polar<FftDecimal> (10.f + static_cast<float> (i), 0.3f);
            second[i] = Polar<FftDecimal> (99.f, 1.7f);
        }

        // first pass captures the locked spectrum
        processor.lockMag();
        processor.lockPhase();
        processor.spectral_process (first, out, bins);

        // second pass should output the locked values, not the new input
        processor.spectral_process (second, out, bins);

        for (int i = 0; i < bins; ++i)
        {
            expectWithinAbsoluteError (out[i].m_mag, first[i].m_mag, 0.01f);
            expectWithinAbsoluteError (out[i].m_phase, first[i].m_phase, sstest::kEpsilon);
        }
    }
};

static PhaseLockTests phaseLockTests;
