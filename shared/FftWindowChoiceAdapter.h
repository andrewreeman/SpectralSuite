/*
  ==============================================================================

    FftWindowChoiceAdapter.h
    Created: 7 Mar 2023 12:02:20am
    Author:  Andrew Reeman

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "FftWindowType.h"

class FftWindowChoiceAdapter: AudioParameterChoice::Listener {
public:
    FftWindowChoiceAdapter();

    int currentIndex() { return (int)m_currentWindow; }
    FftWindowType fftWindow();
    bool shouldChangeFftWindow();
    StringArray fftWindowStrings();
    void listen(AudioParameterChoice* parameter);

    // Inherited via Listener
    void parameterValueChanged(int, float) override;
    void parameterGestureChanged(int, bool) override {};
private:
    bool m_shouldChangeFftWindow;
    FftWindowType m_currentWindow;
    AudioParameterChoice* m_parameter;
};
