#pragma once

#include "JuceHeader.h"
#include "PluginParameters.h"

class AudioValueTreeStateOnLoadListener {
public:
	// Called when setStateInformation is used to replace the state of the PluginParameters
	virtual void onAudioValueTreeStateLoadedFromXmlState(PluginParameters* newState, XmlElement* xmlState) = 0;
    virtual ~AudioValueTreeStateOnLoadListener(){};
};
