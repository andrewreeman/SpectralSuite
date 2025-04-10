//
// Created by Andrew Reeman on 10/04/2025.
//

#pragma once

#include "JuceHeader.h"
#include "../../shared/ParameterContainerComponent.h"
#include "FrequencyShiftPluginParameters.h"
#include "FrequencySlider.h"
//==============================================================================
/*
*/
class SliderContainer : public ParameterContainerComponent
{
public:
	SliderContainer(std::shared_ptr<FrequencyShiftPluginParameters> valueTreeState, Colour textColour, int textBoxHeight);
    ~SliderContainer() override;

    std::shared_ptr<PluginParameters> getPluginParameters() override {
        return pluginParameters;
    }

	const int getComponentHeight() override { return ParameterContainerComponent::getComponentHeight() * SliderContainer::NUM_VERTICAL_COMPONENTS; }
    void paint (Graphics&) override;
    void resized() override;

private:
    static int const NUM_VERTICAL_COMPONENTS = 1;
    std::shared_ptr<PluginParameters> pluginParameters;
	FrequencySlider freqSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliderContainer)
};
