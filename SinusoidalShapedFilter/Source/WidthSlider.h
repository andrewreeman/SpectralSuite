#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../shared/PluginParameters.h"

class WidthSlider : public Component
{
public:
    WidthSlider(std::shared_ptr<PluginParameters> valueTreeState, Colour textColour, int textBoxHeight);
    ~WidthSlider();

    void paint (Graphics&) override;
    void resized() override;

private:
	typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

	Label balanceLabel;
	Slider balance;
	std::unique_ptr<SliderAttachment> balanceAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WidthSlider)
};
