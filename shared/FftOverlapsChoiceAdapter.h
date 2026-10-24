/*
  ==============================================================================

     FftOverlapsChoiceAdapter.h
  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class FftOverlapsChoiceAdapter: AudioParameterChoice::Listener {
public:
    FftOverlapsChoiceAdapter(int initialIndex);

    int currentIndex() { return m_currentIndex; }
    int overlapCount();
    bool shouldChangeFftOverlaps();
    StringArray overlapStrings();
    void listen(AudioParameterChoice* parameter);

    // Inherited via Listener
    void parameterValueChanged(int, float) override;
    void parameterGestureChanged(int, bool) override {}
private:
    // Written from the parameter listener thread; read and cleared on the
    // audio thread, and read again by the FFT switcher thread.
    std::atomic<bool> m_shouldChangeOverlap;
    std::atomic<int> m_currentIndex;
    Array<int> m_overlaps;
    AudioParameterChoice* m_parameter;
};
