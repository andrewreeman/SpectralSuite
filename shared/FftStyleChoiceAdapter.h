/*
  ==============================================================================

    FftStyleChoiceAdapter.h
    Created: 19 Apr 2022 10:45:54am
    Author:  Andrew Reeman

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

enum FftStyle {
    DEFAULT,
    PVOC
};

class FftStyleChoiceAdapter: AudioParameterChoice::Listener {
public:
    FftStyleChoiceAdapter();

    int currentIndex() { return (int)m_currentStyle; }
    FftStyle fftStyle();
    bool shouldChangeFftStyle();
    StringArray fftStyleStrings();
    void listen(AudioParameterChoice* parameter);

    // Inherited via Listener
    void parameterValueChanged(int, float) override;
    void parameterGestureChanged(int, bool) override {};
private:
    bool m_shouldChangeFftStyle;
    FftStyle m_currentStyle;
    AudioParameterChoice* m_parameter;
};
