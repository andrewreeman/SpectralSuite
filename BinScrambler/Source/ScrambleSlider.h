#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class ScrambleSlider    : public Component
{
public:
    ScrambleSlider(AudioProcessorValueTreeState& valueTreeState, Colour textColour, int textBoxHeight);
    ~ScrambleSlider();

    void paint (Graphics&) override;
    void resized() override;

private:
	typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

	Label balanceLabel;
	Slider balance;
	std::unique_ptr<SliderAttachment> balanceAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ScrambleSlider)
};
