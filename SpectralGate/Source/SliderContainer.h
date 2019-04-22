#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../shared/EditorParameterContainer.h"
#include "CutOffSlider.h"
#include "BalanceSlider.h"

//==============================================================================
/*
*/
class SliderContainer : public EditorParameterContainer
{
public:
	SliderContainer(AudioProcessorValueTreeState& valueTreeState, Colour textColour, int textBoxHeight);
    ~SliderContainer();

    void paint (Graphics&) override;
    void resized() override;

private:
	CutOffSlider cutOffSlider;
	BalanceSlider balanceSlider;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliderContainer)
};
