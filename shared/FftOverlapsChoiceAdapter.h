/*
  ==============================================================================

    FftSizeChoiceAdapter.h
    Created: 29 Nov 2018 7:52:08pm
    Author:  rem_d

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class FftOverlapsChoiceAdapter: AudioParameterChoice::Listener {
public:
    FftOverlapsChoiceAdapter(int initialIndex);

    int currentIndex() { return m_currentIndex; }
    int overlapCount();
    bool shouldChangeFftSize();
    StringArray overlapStrings();
    void listen(AudioParameterChoice* parameter);

    // Inherited via Listener
    void parameterValueChanged(int, float) override;
    void parameterGestureChanged(int, bool) override {};
private:
    bool m_shouldChangeOverlap;
    int m_currentIndex;
    Array<int> m_overlaps;
    AudioParameterChoice* m_parameter;
};
