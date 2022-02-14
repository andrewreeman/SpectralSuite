#pragma once

#include "JuceHeader.h"
#include "SettingsPage.h"
#include "ParameterContainerComponent.h"

// TODO: can probably remove this now
class ParameterContainerComponentFactory {
public:
	//virtual ParameterContainerComponent* create(AudioProcessorValueTreeState& treeState, SettingsPage& settingsPageToConfigure) = 0;
	virtual ParameterContainerComponent* create(PluginParameters* treeState) = 0;
	virtual const int getComponentHeight() { return 80; }
    
    virtual ~ParameterContainerComponentFactory(){};
};
