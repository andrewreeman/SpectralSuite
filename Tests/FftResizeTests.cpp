#include <JuceHeader.h>
#include "TestSupport.h"

#include "shared/PhaseBuffer.h"
#include "shared/PhaseVocoder.h"
#include "BinScrambler/Source/BinScramblerFFTProcessor.h"

// Regression tests for issue #97: changing the FFT size at runtime (e.g. when a
// host restores the `fft` parameter on project load) left the STFT write cursor
// (`m_offset`) pointing into the previous buffer size. The next process() pass
// then wrote past the end of the resized m_input / m_output vectors, producing
// garbage magnitudes (extreme, near-Nyquist output) and crashing the host.
class FftResizeTests : public UnitTest
{
public:
    FftResizeTests() : UnitTest ("FFT resize", "SpectralSuite") {}

    void runTest() override
    {
        testOffsetResetOnShrink();
        testOffsetResetOnGrow();
        testWriteStaysInBoundsAfterResize();
    }

private:
    // The write cursor is protected state on StandardFFTProcessor; expose it so
    // the regression can be asserted precisely.
    class InspectableScrambler : public BinScramblerFFTProcessor
    {
    public:
        InspectableScrambler (int size, int hops, int offset, int sRate,
                              std::shared_ptr<PhaseBuffer> phaseBuffer,
                              std::vector<int>* a, std::vector<int>* b)
            : BinScramblerFFTProcessor (size, hops, offset, sRate, phaseBuffer, a, b) {}

        int currentOffset() const { return m_offset; }
        int inputSize() const { return static_cast<int> (m_input.size()); }
        int outputSize() const { return static_cast<int> (m_output.size()); }
    };

    // Advance the write cursor to a non-zero point within a block size so a
    // resize has something stale to inherit.
    static void advance (InspectableScrambler& p, int blockSize, int passes)
    {
        std::vector<float> in (static_cast<size_t> (blockSize), 0.f);
        std::vector<float> out (static_cast<size_t> (blockSize), 0.f);
        for (int i = 0; i < passes; ++i)
            p.process (in.data(), out.data(), blockSize);
    }

    void testOffsetResetOnShrink()
    {
        beginTest ("shrinking the FFT size resets the write cursor");

        std::vector<int> a (2048), b (2048);
        auto phaseBuffer = std::make_shared<PhaseBuffer> (8, 4096);
        InspectableScrambler processor (4096, 1024, 0, 48000, phaseBuffer, &a, &b);

        advance (processor, 1024, 3); // offset -> 3072
        expectEquals (processor.currentOffset(), 3072);

        processor.setFFTSize (2048);

        expectEquals (processor.inputSize(), 2048);
        // The stale 3072 cursor must not survive a shrink to 2048.
        expectEquals (processor.currentOffset(), 0);
    }

    void testOffsetResetOnGrow()
    {
        beginTest ("growing the FFT size resets the write cursor");

        std::vector<int> a (1024), b (1024);
        auto phaseBuffer = std::make_shared<PhaseBuffer> (8, 4096);
        InspectableScrambler processor (2048, 512, 0, 48000, phaseBuffer, &a, &b);

        advance (processor, 512, 3); // offset -> 1536
        expectEquals (processor.currentOffset(), 1536);

        processor.setFFTSize (4096);

        expectEquals (processor.inputSize(), 4096);
        expectEquals (processor.currentOffset(), 0);
    }

    void testWriteStaysInBoundsAfterResize()
    {
        beginTest ("the next write after a resize stays within the new buffers");

        std::vector<int> a (2048), b (2048);
        auto phaseBuffer = std::make_shared<PhaseBuffer> (8, 4096);
        InspectableScrambler processor (4096, 1024, 0, 48000, phaseBuffer, &a, &b);

        advance (processor, 1024, 3);
        processor.setFFTSize (2048);

        const int blockSize = 512; // new hop size for 2048 / 4 overlaps
        expectLessOrEqual (processor.currentOffset() + blockSize, processor.inputSize());
        expectLessOrEqual (processor.currentOffset() + blockSize, processor.outputSize());

        advance (processor, blockSize, 4);
        expectLessOrEqual (processor.currentOffset(), processor.inputSize());
    }
};

static FftResizeTests fftResizeTests;
