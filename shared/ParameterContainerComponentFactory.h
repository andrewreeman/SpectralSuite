#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SettingsPage.h"
#include "ParameterContainerComponent.h"

class ParameterContainerComponentFactory {
public:
	//virtual ParameterContainerComponent* create(AudioProcessorValueTreeState& treeState, SettingsPage& settingsPageToConfigure) = 0;
	virtual ParameterContainerComponent* create(AudioProcessorValueTreeState& treeState) = 0;
	virtual const int getComponentHeight() { return 80; }	
};
