/*
  ==============================================================================

    FftSizeChoiceAdapter.h
    Created: 29 Nov 2018 7:52:08pm
    Author:  rem_d

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class FftSizeChoiceAdapter: AudioParameterChoice::Listener {
public:
	FftSizeChoiceAdapter(int initialIndex);

	int currentIndex() { return m_currentIndex; }
	int fftSize();
	bool shouldChangeFftSize();
	StringArray fftStrings();	
	void listen(AudioParameterChoice* parameter);	
	void remove(Array<int> fftSizes);
	

	// Inherited via Listener
	void parameterValueChanged(int, float) override;
	void parameterGestureChanged(int, bool) override {};
private:	
	bool m_shouldChangeFft;
	int m_currentIndex;
	Array<int> m_ffts;	
	AudioParameterChoice* m_parameter;
};
