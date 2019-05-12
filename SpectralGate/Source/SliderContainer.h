#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../shared/ParameterContainerComponent.h"
#include "CutOffSlider.h"
#include "BalanceSlider.h"

//==============================================================================
/*
*/
class SliderContainer : public ParameterContainerComponent
{
public:
	SliderContainer(std::shared_ptr<PluginParameters> valueTreeState, Colour textColour, int textBoxHeight);
    ~SliderContainer();

	const int getComponentHeight() override { return ParameterContainerComponent::getComponentHeight() * 2; }

    void paint (Graphics&) override;
    void resized() override;

private:
	CutOffSlider cutOffSlider;
	BalanceSlider balanceSlider;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliderContainer)
};
