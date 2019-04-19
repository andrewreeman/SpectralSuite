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
class SettingsPage    : public Component
{
public:
    SettingsPage();
    ~SettingsPage();

    void paint (Graphics&) override;
    void resized() override;

	Array<PropertyComponent*> createSliders(int howMany);

private:
	PropertyPanel propertyPanel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SettingsPage)
};
