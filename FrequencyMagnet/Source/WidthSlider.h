#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../shared/PluginParameters.h"

class BiasSlider : public Component
{
public:
    BiasSlider(std::shared_ptr<PluginParameters> valueTreeState, Colour textColour, int textBoxHeight);
    ~BiasSlider();

    void paint (Graphics&) override;
    void resized() override;

private:
	typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

	Label balanceLabel;
	Slider balance;
	std::unique_ptr<SliderAttachment> balanceAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BiasSlider)
};
