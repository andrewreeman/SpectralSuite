/*
  ==============================================================================

    FrequencySlider.h
    Created: 31 Dec 2018 8:31:43pm
    Author:  rem_d

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
class FrequencySlider : public Component
{
public:
    FrequencySlider(AudioProcessorValueTreeState& valueTreeState, Colour textColour, int textBoxHeight);
    ~FrequencySlider();

    void paint (Graphics&) override;
    void resized() override;	
private:
	typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;		
	Slider frequencyShift;
	std::unique_ptr<SliderAttachment> frequencyShiftAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FrequencySlider)
};
