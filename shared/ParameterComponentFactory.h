#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class ParameterComponentFactory {
public:
	virtual Component* create(AudioProcessorValueTreeState& treeState) = 0;
	virtual const int getComponentHeight() { return 80; }	
};
