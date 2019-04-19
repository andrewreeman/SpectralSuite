#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SettingsPage.h"

class ParameterComponentFactory {
public:
	virtual Component* create(AudioProcessorValueTreeState& treeState, SettingsPage& settingsPageToConfigure) = 0;
	virtual const int getComponentHeight() { return 80; }	
};
