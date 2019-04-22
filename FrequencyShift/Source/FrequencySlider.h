#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../shared/SettingsPage.h"
#include "../../shared/ParameterContainerComponent.h"

//==============================================================================
class FrequencySlider : public ParameterContainerComponent	
{
public:
    FrequencySlider(AudioProcessorValueTreeState& valueTreeState, Colour textColour, int textBoxHeight);
    ~FrequencySlider();

    void paint (Graphics&) override;
    void resized() override;	
	
	Array<PropertyComponent*> getSettingsProperties() override;	
	void onPropertiesChanged() override;	
	void onAudioValueTreeStateLoadedFromXmlState(AudioProcessorValueTreeState & newState, XmlElement* xmlState) override;

private:
	typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;		
	Slider frequencyShift;
	std::unique_ptr<SliderAttachment> frequencyShiftAttachment;
	AudioProcessorValueTreeState* valueTreeState;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FrequencySlider)	
};
