#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "ScatterSlider.h"
#include "ScrambleSlider.h"
#include "RateSlider.h"
#include "../../shared/ParameterContainerComponent.h"
#include "BinScramblerParameters.h"

class SliderContainer : public ParameterContainerComponent
{
public:
	SliderContainer(std::shared_ptr<BinScramblerParameters> valueTreeState, Colour textColour, int textBoxHeight);
    ~SliderContainer();

	const int getComponentHeight() override { return ParameterContainerComponent::getComponentHeight() * 3; }
    void paint (Graphics&) override;
    void resized() override;

	Array<PropertyComponent*> getSettingsProperties() override;
    std::shared_ptr<PluginParameters> getPluginParameters() override { return pluginParameters; }

private:	
	ScatterSlider scatterSlider;
	ScrambleSlider scrambleSlider;
	RateSlider rateSlider;	
	std::shared_ptr<BinScramblerParameters> pluginParameters;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliderContainer)
};
