#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../shared/PluginParameters.h"

class RateSlider : public Component
{
public:
	RateSlider(std::shared_ptr<PluginParameters> valueTreeState, Colour textColour, int textBoxHeight);
	~RateSlider();

	void paint(Graphics&) override;
	void resized() override;

private:
	typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

	Label rateLabel;
	Slider rate;
	std::unique_ptr<SliderAttachment> rateAttachment;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RateSlider)
};
