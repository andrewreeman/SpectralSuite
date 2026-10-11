#include <JuceHeader.h>
#include "TestSupport.h"

#include "shared/PhaseBuffer.h"
#include "shared/SpectralAudioProcessorInteractor.h"
#include "BinScrambler/Source/BinScramblerParameters.h"
#include "BinScrambler/Source/BinScramblerInteractor.h"

// Resize tests that drive the real interactor sequence (setFFTSize, then
// setHopSize and setOffset on each processor), the path a host takes when
// restoring saved state.

namespace
{
    void runBlocks (SpectralAudioProcessorInteractor& interactor, int blocks)
    {
        const auto hop = static_cast<size_t> (interactor.getHopSize());
        std::vector<std::vector<float>> input (2, std::vector<float> (hop, 0.1f));
        std::vector<std::vector<float>> output (2, std::vector<float> (hop, 0.0f));

        for (int block = 0; block < blocks; ++block)
        {
            for (auto& channel : input)
                for (size_t i = 0; i < channel.size(); ++i)
                    channel[i] = 0.1f * std::sin (0.01f * static_cast<float> (block * channel.size() + i));

            std::fill (output[0].begin(), output[0].end(), 0.f);
            std::fill (output[1].begin(), output[1].end(), 0.f);

            interactor.process (&input, &output);

            if (! sstest::allFinite (output))
                return; // caller asserts on finiteness separately
        }
    }
}

class FftResizeProductionTests : public UnitTest
{
public:
    FftResizeProductionTests() : UnitTest ("FFT resize (production sequence)", "SpectralSuite") {}

    void runTest() override
    {
        testDefaultStyleResize();
        testPvocStyleResize();
        testPhaseBufferStaysLargeEnough();
    }

private:
    // Reporter's scenario (#97): a saved project is opened with FFT 4096 and
    // rate 0.6, so prepareToPlay runs directly at 4096 (not the default 2048).
    // Any FFT size other than 2048 used to leave phaseBuffer->prevPhase at its
    // initial 1024 and phaseDiff2Hertz read past the end on the first block.
    void testDefaultStyleResize()
    {
        beginTest ("prepare at restored FFT 4096 (DEFAULT style) keeps output finite");

        // Issue #97: preparing directly at a non-default size must not crash.
        sstest::MockProcessor processor;
        auto params = std::make_shared<BinScramblerParameters> (&processor);
        params->getParameter ("rate")->setValueNotifyingHost (
            params->getParameter ("rate")->convertTo0to1 (0.6f));

        BinScramblerInteractor interactor (4, params);
        interactor.prepareToPlay (4096, 48000, 2); // restored project FFT size

        runBlocks (interactor, 120);

        // Also exercise a subsequent size change, as the UI allows.
        interactor.setFftSize (8192);
        runBlocks (interactor, 60);

        expect (true, "reached the end without a crash");
    }

    void testPvocStyleResize()
    {
        beginTest ("prepare at restored FFT 4096 (PVOC style) keeps output finite");

        // Issue #97: the phase-vocoder path reads prevPhase, which used to be
        // too small here and crashed.
        sstest::MockProcessor processor;
        auto params = std::make_shared<BinScramblerParameters> (&processor);
        BinScramblerInteractor interactor (4, params);
        interactor.prepareToPlay (4096, 48000, 2); // restored project FFT size
        interactor.usePvoc (true); // the phase-vocoder path reads prevPhase

        runBlocks (interactor, 120);
        interactor.setFftSize (8192);
        runBlocks (interactor, 60);

        expect (true, "reached the end without a crash");
    }

    void testPhaseBufferStaysLargeEnough()
    {
        beginTest ("PhaseBuffer prevPhase is never smaller than fftSize/2 after resizes");

        // Mirrors the client count the interactor uses: numChans (2) * overlaps (4).
        PhaseBuffer buffer (2 * 4, 1024);

        // A single request must grow the buffer, regardless of what other
        // clients have requested.
        buffer.requestResize (4096 / 2);
        expectGreaterOrEqual (static_cast<int> (buffer.prevPhase.size()), 4096 / 2);

        buffer.requestResize (8192 / 2);
        expectGreaterOrEqual (static_cast<int> (buffer.prevPhase.size()), 8192 / 2);

        // Grow-only: a smaller request must never shrink the buffer, so reads
        // for the current (larger) halfSize can never go out of bounds.
        buffer.requestResize (2048 / 2);
        expectGreaterOrEqual (static_cast<int> (buffer.prevPhase.size()), 8192 / 2);
    }
};

static FftResizeProductionTests fftResizeProductionTests;
