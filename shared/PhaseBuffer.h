#ifndef PhaseBuffer_h
#define PhaseBuffer_h

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
// The previous count-based barrier required every client to request the same
// size before resizing. On initial prepare that never happened (the plugin's
// setFftSize early-returns when the size is unchanged), so prevPhase stayed at
// its initial size and phaseDiff2Hertz read out of bounds for any FFT size
// other than the default 2048 (issue #97).
class PhaseBuffer {
public:
    PhaseBuffer(int clientCount, int initialSize) : m_usePvoc(false) {
        (void) clientCount; // retained for API compatibility; barrier no longer needed
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
    bool m_usePvoc;
};

#endif /* PhaseBuffer_h */
