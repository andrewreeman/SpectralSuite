#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "FreqSlider.h"
#include "WidthSlider.h"
#include "BiasSlider.h"
#include "FrequencyMagnetParameters.h"
#include "../../shared/ParameterContainerComponent.h"

//==============================================================================
/*
*/
class SliderContainer : public ParameterContainerComponent
{
public:
	SliderContainer(std::shared_ptr<FrequencyMagnetParameters> valueTreeState, Colour textColour, int textBoxHeight);
    ~SliderContainer();

    std::shared_ptr<PluginParameters> getPluginParameters() override { return params; }
	const int getComponentHeight() override { return ParameterContainerComponent::getComponentHeight() * 3; }
    void paint (Graphics&) override;
    void resized() override;
	Array<PropertyComponent*> getSettingsProperties() override;			
    void onPropertiesChanged() override;
    void onAudioValueTreeStateLoadedFromXmlState(PluginParameters* newState, XmlElement* xmlState) override;

private:
	FreqSlider freqSlider;
	PhaseSlider phaseSlider;
	BiasSlider widthSlider;
	std::shared_ptr<FrequencyMagnetParameters> params;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliderContainer)
};
