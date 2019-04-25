#pragma once

#include "JuceHeader.h"
#include "../../shared/SettingsPage.h"
#include "../../shared/ParameterContainerComponent.h"
//#include "../../shared/PluginParameters.h"

//==============================================================================
class FrequencySlider : public ParameterContainerComponent	
{
public:
    FrequencySlider(std::shared_ptr<PluginParameters> valueTreeState, Colour textColour, int textBoxHeight);
    ~FrequencySlider();

    void paint (Graphics&) override;
    void resized() override;	
	
	Array<PropertyComponent*> getSettingsProperties() override;	
	void onPropertiesChanged() override;	
	void onAudioValueTreeStateLoadedFromXmlState(PluginParameters* newState, XmlElement* xmlState) override;

private:
	typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;		
	Slider frequencyShift;
	std::unique_ptr<SliderAttachment> frequencyShiftAttachment;
	std::shared_ptr<PluginParameters> valueTreeState;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FrequencySlider)	
};
