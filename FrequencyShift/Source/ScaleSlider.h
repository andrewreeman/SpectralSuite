#pragma once

#include "JuceHeader.h"
#include "FrequencyShiftPluginParameters.h"

class ScaleSlider final : public Component
{
public:
    ScaleSlider(std::shared_ptr<FrequencyShiftPluginParameters> aValueTreeState, Colour textColour, int textBoxHeight);
    ~ScaleSlider() override;

    void paint (Graphics&) override;
    void resized() override;

private:
	typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
	Label scaleLabel;
	Slider scaleSlider;
	std::unique_ptr<SliderAttachment> scaleAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ScaleSlider)
};
