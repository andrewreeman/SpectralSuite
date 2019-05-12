#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "ScatterSlider.h"
#include "ScrambleSlider.h"
#include "RateSlider.h"
#include "../../shared/ParameterContainerComponent.h"

//==============================================================================
/*
*/
class SliderContainer : public ParameterContainerComponent
{
public:
	SliderContainer(std::shared_ptr<PluginParameters> valueTreeState, Colour textColour, int textBoxHeight);
    ~SliderContainer();

	const int getComponentHeight() override { return ParameterContainerComponent::getComponentHeight() * 3; }
    void paint (Graphics&) override;
    void resized() override;

private:	
	ScatterSlider scatterSlider;
	ScrambleSlider scrambleSlider;
	RateSlider rateSlider;	
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliderContainer)
};
