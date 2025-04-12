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
	SliderContainer(const std::shared_ptr<FrequencyShiftPluginParameters> &valueTreeState, Colour textColour, int textBoxHeight);
    ~SliderContainer() override;

    std::shared_ptr<PluginParameters> getPluginParameters() override {
        return pluginParameters;
    }

	const int getComponentHeight() override { return ParameterContainerComponent::getComponentHeight() * SliderContainer::NUM_VERTICAL_COMPONENTS; }
    void paint (Graphics&) override;
    void resized() override;

private:
    static constexpr int NUM_VERTICAL_COMPONENTS = 2;
	typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

    std::shared_ptr<PluginParameters> pluginParameters;
	FrequencySlider freqSlider;
    Slider scaleSlider;
	std::unique_ptr<SliderAttachment> scaleAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliderContainer)
};
