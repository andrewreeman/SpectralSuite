/*
  ==============================================================================

    SettingsPage.h
    Created: 17 Apr 2019 9:48:04pm
    Author:  rem_d

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class SettingsPage : public Component
{
public:
	class Listener {
	public:
		virtual void onPropertiesChanged() = 0;
	};

    SettingsPage();
    ~SettingsPage();

    void paint (Graphics&) override;
    void resized() override;
	void setListener(Listener* listener) { this->listener = listener; }

	Array<PropertyComponent*> createSliders(int howMany);

private:
	void backButtonClicked();

	PropertyPanel propertyPanel;
	DrawableButton backButton;
	Listener* listener;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SettingsPage)
};
