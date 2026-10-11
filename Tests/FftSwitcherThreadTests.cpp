#include <JuceHeader.h>
#include "TestSupport.h"

#include "shared/SpectralAudioPlugin.h"
#include "shared/SpectralAudioProcessorInteractor.h"
#include "shared/PhaseBuffer.h"
#include "BinScrambler/Source/BinScramblerParameters.h"
#include "BinScrambler/Source/BinScramblerInteractor.h"

#include <atomic>
#include <thread>

// FFT switcher-thread exclusion tests: the audio thread must never observe a
// half-mutated processor bank, must never block on the switcher, and must
// emit silence for a hop taken while a switch holds the process lock.
// The plugin-level tests drive the real SpectralAudioPlugin, so the full
// production path is exercised: parameter change -> audio thread dispatch ->
// switcher thread mutation under the blocking lock -> try-locked hop.

namespace
{
    using sstest::allFinite;
    using sstest::allZero;
    using sstest::makeSineBuffer;

    // Builds a real plugin around a BinScrambler interactor and keeps a raw
    // pointer to the interactor so tests can hold its process lock.
    class SwitcherTestFactory : public SpectralAudioPlugin::DependencyFactory
    {
    public:
        std::shared_ptr<PluginParameters> createParams (SpectralAudioPlugin* plugin) override
        {
            m_params = std::make_shared<BinScramblerParameters> (plugin);
            return m_params;
        }

        ParameterContainerComponent* createUi (SpectralAudioPlugin*) override { return nullptr; }

        std::unique_ptr<SpectralAudioProcessorInteractor> createProcessor (SpectralAudioPlugin*) override
        {
            auto interactor = std::make_unique<BinScramblerInteractor> (4, m_params);
            m_interactor = interactor.get();
            return interactor;
        }

        std::shared_ptr<BinScramblerParameters> m_params;
        SpectralAudioProcessorInteractor* m_interactor = nullptr;
    };

    void feedBlocks (SpectralAudioPlugin& plugin, int blocks, int& blockIndex)
    {
        MidiBuffer midi;
        for (int i = 0; i < blocks; ++i)
        {
            auto buffer = makeSineBuffer (2, 64, blockIndex++);
            plugin.processBlock (buffer, midi);
        }
    }
}

class FftSwitcherThreadTests : public UnitTest
{
public:
    FftSwitcherThreadTests() : UnitTest ("FFT switcher thread exclusion", "SpectralSuite") {}

    void runTest() override
    {
        testSilenceWhileSwitchHoldsLock();
        testConcurrentSwitchHammer();
        testInteractorConcurrentMutation();
        testDestructorJoinsInFlightSwitch();
    }

private:
    void testSilenceWhileSwitchHoldsLock()
    {
        beginTest ("a hop during a switch emits silence and never blocks");
        // Issue #105: with no exclusion the hop processed normally during a
        // mutation; the try-locked fallback must emit silence instead.

        // The plugin takes ownership of the factory and deletes it.
        auto* factory = new SwitcherTestFactory();
        SpectralAudioPlugin plugin (factory);
        plugin.prepareToPlay (48000.0, 64);

        int blockIndex = 0;
        feedBlocks (plugin, 40, blockIndex);
        MidiBuffer midi;

        // Sanity: a normal block produces finite, non-silent output, so the
        // silence check below cannot pass vacuously.
        {
            auto buffer = makeSineBuffer (2, 64, blockIndex++);
            plugin.processBlock (buffer, midi);
            expect (allFinite (buffer), "output must stay finite");
            expect (! allZero (buffer), "warmed-up plugin must produce output");
        }

        // A background thread holds the process lock exactly as the switcher
        // thread does mid-switch. (A same-thread hold would not work: the
        // underlying mutex is recursive, so processBlock would still acquire.)
        std::atomic<bool> lockHeld { false };
        std::atomic<bool> releaseLock { false };
        std::thread lockHolder ([&]()
        {
            const CriticalSection::ScopedLockType lock (factory->m_interactor->getProcessLock());
            lockHeld = true;
            while (! releaseLock.load())
                Thread::sleep (2);
        });

        while (! lockHeld.load())
            Thread::sleep (1);

        {
            auto buffer = makeSineBuffer (2, 64, blockIndex++);
            plugin.processBlock (buffer, midi);
            expect (allZero (buffer), "a hop during a switch must emit silence");
        }

        releaseLock = true;
        lockHolder.join();

        // Processing resumes once the switch releases the lock.
        {
            auto buffer = makeSineBuffer (2, 64, blockIndex++);
            plugin.processBlock (buffer, midi);
            expect (allFinite (buffer), "output must stay finite after the switch");
        }
    }

    void testConcurrentSwitchHammer()
    {
        beginTest ("concurrent processBlock and dispatched FFT switches stay crash-free and finite");

        // The plugin takes ownership of the factory and deletes it.
        auto* factory = new SwitcherTestFactory();
        SpectralAudioPlugin plugin (factory);
        plugin.prepareToPlay (48000.0, 64);

        std::atomic<bool> finished { false };
        std::atomic<bool> outputWasFinite { true };

        std::thread audioThread ([&]()
        {
            MidiBuffer midi;
            int blockIndex = 0;
            while (! finished.load())
            {
                auto buffer = makeSineBuffer (2, 64, blockIndex++);
                plugin.processBlock (buffer, midi);
                if (! allFinite (buffer))
                    outputWasFinite = false;
                Thread::sleep (1); // host-like block cadence
            }
        });

        // Flip the FFT size parameter back and forth. Each change is picked
        // up by processBlock and dispatched to the switcher thread, exactly as
        // host automation does.
        auto* fftChoice = factory->m_params->getParameterChoice ("fft");
        for (int flip = 0; flip < 30; ++flip)
        {
            const float index = (flip % 2 == 0) ? 5.f : 4.f; // 4096 <-> 2048
            fftChoice->setValueNotifyingHost (fftChoice->convertTo0to1 (index));
            Thread::sleep (20);
        }

        Thread::sleep (200); // let the last switch settle
        finished = true;
        audioThread.join();

        expect (outputWasFinite.load(), "output must stay finite across switches");

        int blockIndex = 0;
        feedBlocks (plugin, 40, blockIndex);
        {
            MidiBuffer midi;
            auto buffer = makeSineBuffer (2, 64, blockIndex++);
            plugin.processBlock (buffer, midi);
            expect (allFinite (buffer), "output must stay finite after the hammer");
        }
    }

    void testInteractorConcurrentMutation()
    {
        beginTest ("interactor process() concurrent with setFftSize/setNumOverlaps stays crash-free");

        sstest::MockProcessor processor;
        auto params = std::make_shared<BinScramblerParameters> (&processor);
        BinScramblerInteractor interactor (4, params);
        interactor.prepareToPlay (2048, 48000, 2);

        // Sized for the largest hop the mutations below can produce
        // (4096 / 4 = 1024).
        std::vector<std::vector<float>> input (2, std::vector<float> (1024, 0.1f));
        std::vector<std::vector<float>> output (2, std::vector<float> (1024, 0.0f));

        std::atomic<bool> finished { false };

        std::thread mutator ([&]()
        {
            // Emulates the switcher thread: mutations hold the process lock.
            for (int i = 0; i < 200; ++i)
            {
                const CriticalSection::ScopedLockType lock (interactor.getProcessLock());
                interactor.setFftSize ((i % 2 == 0) ? 4096 : 2048);
                interactor.setNumOverlaps ((i % 3 == 0) ? 8 : 4);
            }
            finished = true;
        });

        bool allOutputsFinite = true;
        while (! finished.load())
        {
            std::fill (output[0].begin(), output[0].end(), 0.f);
            std::fill (output[1].begin(), output[1].end(), 0.f);

            // Emulates the audio thread protocol: try-lock, skip the hop on
            // contention instead of blocking. The sleep keeps the cadence
            // host-like; a greedy spin would starve the mutator thread.
            {
                const CriticalSection::ScopedTryLockType lock (interactor.getProcessLock());
                if (lock.isLocked())
                {
                    interactor.process (&input, &output);
                    if (! sstest::allFinite (output))
                        allOutputsFinite = false;
                }
            }

            Thread::sleep (1);
        }

        mutator.join();

        expect (allOutputsFinite, "output must stay finite during concurrent mutations");

        // The bank must still be in a working state afterwards.
        const CriticalSection::ScopedLockType lock (interactor.getProcessLock());
        std::fill (output[0].begin(), output[0].end(), 0.f);
        std::fill (output[1].begin(), output[1].end(), 0.f);
        interactor.process (&input, &output);
        expect (sstest::allFinite (output), "output must stay finite after concurrent mutations");
    }

    void testDestructorJoinsInFlightSwitch()
    {
        beginTest ("destroying the plugin joins an in-flight switch cleanly");
        // Issue #105: the destructor force-killed the switcher thread
        // mid-mutation; it must join instead.

        for (int attempt = 0; attempt < 3; ++attempt)
        {
            // The plugin takes ownership of the factory and deletes it.
            auto* factory = new SwitcherTestFactory();
            {
                SpectralAudioPlugin plugin (factory);
                plugin.prepareToPlay (48000.0, 64);

                // Point the adapter at a large size other than the prepared
                // one, then dispatch the switch through processBlock so the
                // switcher thread runs it in the background.
                auto* fftChoice = factory->m_params->getParameterChoice ("fft");
                fftChoice->setValueNotifyingHost (fftChoice->convertTo0to1 (8.f)); // 32768
                auto buffer = makeSineBuffer (2, 64, attempt);
                MidiBuffer midi;
                plugin.processBlock (buffer, midi); // dispatches the switch

                // Destroy likely while the switcher thread is mid-mutation.
            }
        }

        expect (true, "destroyed without hanging or crashing");
    }
};

static FftSwitcherThreadTests fftSwitcherThreadTests;
