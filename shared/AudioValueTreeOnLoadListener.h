/*
  ==============================================================================

    AudioValueTreeOnLoadListener.h
    Created: 21 Apr 2019 11:26:14am
    Author:  rem_d

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class AudioValueTreeStateOnLoadListener {
public:
	// Called when setStateInformation is used to replace the state of the AudioValueTreeState
	virtual void onAudioValueTreeStateLoaded(AudioProcessorValueTreeState& newState) = 0;
};