#ifndef PhaseBuffer_h
#define PhaseBuffer_h

#include <atomic>
#include <vector>

// Holds the previous phase values, shared between all overlapping STFT
// instances of a plugin.
//
// phaseDiff2Hertz reads this by bin index up to (fftSize / 2), so it must
// always be at least as large as the largest half-FFT size in use. Resizing is
// immediate and grow-only: shrinking is unnecessary (reads never exceed the
// current halfSize) and always keeping the high-water mark means a resize can
// never leave the buffer smaller than what a live processor will read.
//
// prevPhase and its resizes are serialized by the owning interactor's process
// lock: the audio thread reads/writes per bin under the try-locked hop, and
// the FFT switcher thread resizes under the blocking lock. m_usePvoc is
// written from the switcher thread and read on the audio thread, so it is
// atomic.
class PhaseBuffer {
public:
    PhaseBuffer(int clientCount, int initialSize) : m_usePvoc(false) {
        (void) clientCount; // unused; retained for API compatibility
        prevPhase.assign((size_t)initialSize, 0.f);
    }

    bool isAvailable() {
        return m_usePvoc;
    }

    void requestResize(int newSize) {
        if (newSize <= 0) { return; }
        if ((size_t) newSize <= prevPhase.size()) { return; }

        prevPhase.resize((size_t) newSize, 0.f);
    }

    void setUsePvoc(bool shouldUsePvoc) {
        m_usePvoc = shouldUsePvoc;
    }

    // The overlapping instances of STFT will all write their previous phase values to this static array.
    std::vector<float> prevPhase;

private:
    std::atomic<bool> m_usePvoc;
};

#endif /* PhaseBuffer_h */
