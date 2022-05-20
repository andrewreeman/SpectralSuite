#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "FreqSlider.h"
#include "WidthSlider.h"
#include "PhaseSlider.h"
#include "../../shared/ParameterContainerComponent.h"

class SliderContainer : public ParameterContainerComponent
{
public:
	SliderContainer(std::shared_ptr<PluginParameters> valueTreeState, Colour textColour, int textBoxHeight);
    ~SliderContainer();
    
    std::shared_ptr<PluginParameters> getPluginParameters() override {
        return valueTreeState;
    }

	const int getComponentHeight() override { return ParameterContainerComponent::getComponentHeight() * 3; }
    void paint (Graphics&) override;
    void resized() override;

private:
    std::shared_ptr<PluginParameters> valueTreeState;
	FreqSlider freqSlider;
	PhaseSlider phaseSlider;
	WidthSlider widthSlider;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliderContainer)
};
