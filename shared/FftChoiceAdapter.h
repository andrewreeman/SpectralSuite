/*
  ==============================================================================

    FftChoiceAdapter.h
    Created: 29 Nov 2018 7:52:08pm
    Author:  rem_d

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class FftChoiceAdapter: AudioParameterChoice::Listener {
public:
	FftChoiceAdapter(int initialIndex);

	int currentIndex() { return m_currentIndex; }
	int fftSize();
	bool shouldChangeFft();
	StringArray fftStrings();	
	void listen(AudioParameterChoice* parameter);	
	void remove(Array<int> fftSizes);
	

	// Inherited via Listener
	void parameterValueChanged(int parameterIndex, float newValue) override;
	void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override {};
private:	
	bool m_shouldChangeFft;
	int m_currentIndex;
	Array<int> m_ffts;	
	AudioParameterChoice* m_parameter;
};