/*
  ==============================================================================

    FrequencySlider.h
    Created: 31 Dec 2018 8:31:43pm
    Author:  rem_d

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../shared/SettingsPage.h"

//==============================================================================
class FrequencySlider : public Component, 
	public SettingsPage::Listener
{
public:
    FrequencySlider(AudioProcessorValueTreeState& valueTreeState, Colour textColour, int textBoxHeight);
    ~FrequencySlider();

    void paint (Graphics&) override;
    void resized() override;	

	Array<PropertyComponent*> getSettingsProperties();
private:
	typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;		
	Slider frequencyShift;
	std::unique_ptr<SliderAttachment> frequencyShiftAttachment;
	AudioProcessorValueTreeState* valueTreeState;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FrequencySlider)

		// Inherited via Listener
		virtual void onPropertiesChanged() override;
};
