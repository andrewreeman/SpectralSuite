/*
  ==============================================================================

    BalanceSlider.h
    Created: 1 Jan 2019 2:54:30pm
    Author:  rem_d

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class BalanceSlider    : public Component
{
public:
    BalanceSlider(AudioProcessorValueTreeState& valueTreeState, Colour textColour, int textBoxHeight);
    ~BalanceSlider();

    void paint (Graphics&) override;
    void resized() override;

private:
	typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

	Label balanceLabel;
	Slider balance;
	std::unique_ptr<SliderAttachment> balanceAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BalanceSlider)
};
