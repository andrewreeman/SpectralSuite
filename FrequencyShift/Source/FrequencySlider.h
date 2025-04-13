#pragma once

#include "JuceHeader.h"
#include "FrequencyShiftPluginParameters.h"
#include "../../shared/ParameterContainerComponent.h"

class FrequencySlider final : public ParameterContainerComponent
{
public:
    FrequencySlider(std::shared_ptr<FrequencyShiftPluginParameters> aValueTreeState, Colour textColour, int textBoxHeight);
    ~FrequencySlider() override;
    
    std::shared_ptr<PluginParameters> getPluginParameters() override {
        return valueTreeState;
    }

    void paint (Graphics&) override;
    void resized() override;	
	
	Array<PropertyComponent*> getSettingsProperties() override;	
	void onPropertiesChanged() override;	
	void onAudioValueTreeStateLoadedFromXmlState(PluginParameters* newState, XmlElement* xmlState) override;
    
private:
	typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
	Label frequencyShiftLabel;
	Slider frequencyShift;
	std::unique_ptr<SliderAttachment> frequencyShiftAttachment;
	std::shared_ptr<FrequencyShiftPluginParameters> valueTreeState;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FrequencySlider)	
};
