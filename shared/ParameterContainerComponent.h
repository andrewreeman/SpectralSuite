#pragma once

#include "JuceHeader.h"
#include "SettingsPage.h"
#include "AudioValueTreeOnLoadListener.h"
#include "PluginParameters.h"

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

	virtual const int getComponentHeight() { return 80; }
	virtual void paint(Graphics&) = 0;
	virtual void resized() = 0;

	virtual Array<PropertyComponent*> getSettingsProperties() {
		return Array<PropertyComponent*>();
	}

	// Inherited via Listener
	virtual void onPropertiesChanged() override {}

	// Inherited via AudioValueTreeStateOnLoadListener
	virtual void onAudioValueTreeStateLoadedFromXmlState(PluginParameters*, XmlElement*) override {}

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParameterContainerComponent)	
};
