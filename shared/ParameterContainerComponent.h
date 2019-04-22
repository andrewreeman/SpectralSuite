/*
  ==============================================================================

    ParameterContainerComponent.h
    Created: 21 Apr 2019 11:10:40am
    Author:  rem_d

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SettingsPage.h"
#include "AudioValueTreeOnLoadListener.h"

//==============================================================================
/*
*/
class ParameterContainerComponent : 
	public Component,
	public SettingsPage::Listener,
	public AudioValueTreeStateOnLoadListener	
{
public:
    ParameterContainerComponent();
    ~ParameterContainerComponent();

	virtual void paint(Graphics&) = 0;
	virtual void resized() = 0;

	virtual Array<PropertyComponent*> getSettingsProperties() {
		return Array<PropertyComponent*>();
	}

	// Inherited via Listener
	virtual void onPropertiesChanged() override {}

	// Inherited via AudioValueTreeStateOnLoadListener
	virtual void onAudioValueTreeStateLoadedFromXmlState(AudioProcessorValueTreeState & newState, XmlElement* xmlState) override {}

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParameterContainerComponent)	
};
