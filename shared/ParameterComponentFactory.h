#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SettingsPage.h"
#include "EditorParameterContainer.h"

class ParameterComponentFactory {
public:
	//virtual EditorParameterContainer* create(AudioProcessorValueTreeState& treeState, SettingsPage& settingsPageToConfigure) = 0;
	virtual EditorParameterContainer* create(AudioProcessorValueTreeState& treeState) = 0;
	virtual const int getComponentHeight() { return 80; }	
};
