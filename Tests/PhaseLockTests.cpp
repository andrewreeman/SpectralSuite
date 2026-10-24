#include <JuceHeader.h>
#include <atomic>
#include <cmath>
#include <thread>
#include "TestSupport.h"

#include "shared/PhaseBuffer.h"
#include "shared/PhaseVocoder.h"
#include "shared/SpectralAudioProcessorInteractor.h"
#include "PhaseLock/Source/LockState.h"
#include "PhaseLock/Source/TransitionState.h"
#include "PhaseLock/Source/PhaseLockFFTProcessor.h"
#include "PhaseLock/Source/PhaseLockInteractor.h"
#include "PhaseLock/Source/PhaseLockParameters.h"

namespace
{
    // Minimal concrete AudioProcessor so the parameters can be constructed,
    // mirroring FftResizeProductionTests.
    class PhaseLockMockProcessor : public AudioProcessor
    {
    public:
        PhaseLockMockProcessor()
            : AudioProcessor (BusesProperties()
                                  .withInput ("In", AudioChannelSet::stereo(), true)
                                  .withOutput ("Out", AudioChannelSet::stereo(), true)) {}

        const String getName() const override { return "PhaseLockMock"; }
        void prepareToPlay (double, int) override {}
        void releaseResources() override {}
        bool isBusesLayoutSupported (const BusesLayout&) const override { return true; }
        void processBlock (AudioBuffer<float>&, MidiBuffer&) override {}
        double getTailLengthSeconds() const override { return 0.0; }
        bool acceptsMidi() const override { return false; }
        bool producesMidi() const override { return false; }
        AudioProcessorEditor* createEditor() override { return nullptr; }
        bool hasEditor() const override { return false; }
        int getNumPrograms() override { return 1; }
        int getCurrentProgram() override { return 0; }
        void setCurrentProgram (int) override {}
        const String getProgramName (int) override { return {}; }
        void changeProgramName (int, const String&) override {}
        void getStateInformation (MemoryBlock&) override {}
        void setStateInformation (const void*, int) override {}
    };

    bool outputAllFinite (const std::vector<std::vector<float>>& buffer)
    {
        for (const auto& channel : buffer)
            for (float sample : channel)
                if (! std::isfinite (sample))
                    return false;
        return true;
    }

    void setPhaseLockToggle (PhaseLockParameters& params, const char* paramId, bool on)
    {
        if (auto* param = params.getParameter (paramId))
            param->setValueNotifyingHost (on ? 1.0f : 0.0f);
    }

    // One hop-block through the real interactor (prepareProcess per overlap,
    // then each overlap's STFT), filled with a low-level sine like a live input.
    bool runOnePhaseLockBlock (SpectralAudioProcessorInteractor& interactor, int block)
    {
        const auto hop = static_cast<size_t> (interactor.getHopSize());
        std::vector<std::vector<float>> input (2, std::vector<float> (hop, 0.0f));
        std::vector<std::vector<float>> output (2, std::vector<float> (hop, 0.0f));

        for (size_t chan = 0; chan < input.size(); ++chan)
            for (size_t i = 0; i < hop; ++i)
                input[chan][i] = 0.1f * std::sin (0.01f * static_cast<float> (block * hop + i));

        interactor.process (&input, &output);
        return outputAllFinite (output);
    }
}

class PhaseLockTests : public UnitTest
{
public:
    PhaseLockTests() : UnitTest ("PhaseLock processor", "SpectralSuite") {}

    void runTest() override
    {
        testLockStateTransitions();
        testTransitionStateRamp();
        testLockCapturesAndHoldsSpectrum();
        testRapidToggleLocksAndMorph();
        testRapidToggleProcessorDirect();
        testMorphBlendWithSmallerOutputStaysInBounds();
        testConcurrentToggleFromAnotherThread();
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

    // Issue #101: toggling the Phase Lock / Frequency Lock / Morph switches
    // rapidly must never crash and must keep output finite. Drives the real
    // interactor (prepareToPlay -> per-block prepareProcess + STFT) with the
    // switches flipping almost every hop, including across FFT size changes.
    void testRapidToggleLocksAndMorph()
    {
        beginTest ("rapid lock/morph toggling through the interactor keeps output finite");

        PhaseLockMockProcessor processor;
        auto params = std::make_shared<PhaseLockParameters> (&processor);
        PhaseLockInteractor interactor (4, params);
        interactor.prepareToPlay (2048, 48000, 2);

        const std::vector<int> sizes { 2048, 4096, 8192 };
        bool finite = true;

        for (int pass = 0; pass < 2; ++pass)
        {
            if (pass == 1)
                interactor.usePvoc (true); // the phase-vocoder path reads prevPhase

            for (int block = 0; block < 300 && finite; ++block)
            {
                setPhaseLockToggle (*params, "phaselock", (block % 3) == 0);
                setPhaseLockToggle (*params, "maglock", (block % 5) == 0);
                setPhaseLockToggle (*params, "morphmagphase", (block % 7) == 0);

                if (block % 40 == 0)
                    interactor.setFftSize (sizes[static_cast<size_t> (block / 40) % sizes.size()]);

                finite = runOnePhaseLockBlock (interactor, block);
            }
        }

        expect (finite, "output stayed finite through rapid toggling");
    }

    // Issue #101, processor level: hammer the lock/morph state machines and
    // FFT resizes directly around spectral_process, including bins changing
    // under a held lock. Must never throw or read out of bounds.
    void testRapidToggleProcessorDirect()
    {
        beginTest ("rapid processor-level lock toggling across FFT resizes stays in bounds");

        auto buffer = std::make_shared<PhaseBuffer> (4, 8192);
        const std::vector<int> sizes { 512, 1024, 2048 };

        PhaseLockFFTProcessor processor (512, 128, 0, 48000, buffer);
        processor.setMagTrack (0.f);

        bool finite = true;
        int currentSize = 512; // tracks the processor's FFT size, as in production

        for (int step = 0; step < 600 && finite; ++step)
        {
            const int size = sizes[static_cast<size_t> (step) % sizes.size()];

            if (step % 9 == 0)
            {
                processor.setFFTSize (size);
                currentSize = size;
            }

            const int bins = currentSize / 2;

            if ((step % 2) == 0)
                processor.lockPhase();
            else
                processor.unlockPhase();

            if ((step % 3) == 0)
                processor.lockMag();
            else
                processor.unlockMag();

            if ((step % 4) == 0)
                processor.startMorphingMagAndFreq();
            else
                processor.stopMorphingMagAndFreq();

            PolarVector in (static_cast<size_t> (bins)), out (static_cast<size_t> (bins));
            for (int i = 0; i < bins; ++i)
                in[static_cast<size_t> (i)] = Polar<FftDecimal> (1.0f + 0.01f * static_cast<float> (i), 0.2f);

            processor.spectral_process (in, out, bins);

            for (int i = 0; i < bins; ++i)
            {
                if (! std::isfinite (out[static_cast<size_t> (i)].m_mag)
                    || ! std::isfinite (out[static_cast<size_t> (i)].m_phase))
                {
                    finite = false;
                    break;
                }
            }
        }

        expect (finite, "output stayed finite through processor-level toggling");
    }

    // Issue #101: doMorphTransition used to bound its blend loop by the stale
    // captured target size instead of the output buffer, reading past the end
    // of a smaller output. The blend must never index past the output given
    // to spectral_process.
    void testMorphBlendWithSmallerOutputStaysInBounds()
    {
        beginTest ("morph blend with a smaller output than the captured target stays in bounds");

        auto buffer = std::make_shared<PhaseBuffer> (4, 8192);
        PhaseLockFFTProcessor processor (2048, 512, 0, 48000, buffer);
        processor.setMagTrack (0.f);

        // Capture morph targets at the full 1024 bins, then blend into a
        // smaller output while the transition is still running.
        PolarVector bigIn (1024), bigOut (1024);
        for (size_t i = 0; i < bigIn.size(); ++i)
            bigIn[i] = Polar<FftDecimal> (1.0f, 0.2f);

        processor.startMorphingMagAndFreq();
        processor.spectral_process (bigIn, bigOut, 1024);

        PolarVector smallIn (256), smallOut (256);
        for (size_t i = 0; i < smallIn.size(); ++i)
            smallIn[i] = Polar<FftDecimal> (1.0f, 0.2f);

        processor.spectral_process (smallIn, smallOut, 256);

        bool finite = true;
        for (const auto& bin : smallOut)
            finite = finite && std::isfinite (bin.m_mag) && std::isfinite (bin.m_phase);

        expect (finite, "smaller blended output stayed finite");
    }

    // Issue #101: the switches live on the message thread in production while
    // prepareProcess reads them on the audio thread. Hammer the parameters
    // from a second thread while blocks run; this must never crash.
    void testConcurrentToggleFromAnotherThread()
    {
        beginTest ("toggling lock params from another thread while processing never crashes");

        PhaseLockMockProcessor processor;
        auto params = std::make_shared<PhaseLockParameters> (&processor);
        PhaseLockInteractor interactor (4, params);
        interactor.prepareToPlay (2048, 48000, 2);

        std::atomic<bool> stop { false };
        std::thread toggler ([&]
        {
            int i = 0;
            while (! stop.load())
            {
                setPhaseLockToggle (*params, "phaselock", (i % 2) == 0);
                setPhaseLockToggle (*params, "maglock", (i % 3) == 0);
                setPhaseLockToggle (*params, "morphmagphase", (i % 5) == 0);
                ++i;
            }
        });

        bool finite = true;
        for (int block = 0; block < 200 && finite; ++block)
            finite = runOnePhaseLockBlock (interactor, block);

        stop.store (true);
        toggler.join();

        expect (finite, "output stayed finite with concurrent toggling");
    }
};

static PhaseLockTests phaseLockTests;
