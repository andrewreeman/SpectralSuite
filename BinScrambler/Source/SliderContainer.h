#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "ScatterSlider.h"
#include "ScrambleSlider.h"
#include "RateSlider.h"
#include "../../shared/ParameterContainerComponent.h"

//==============================================================================
/*
*/
class SliderContainer : public ParameterContainerComponent
{
public:
	SliderContainer(AudioProcessorValueTreeState& valueTreeState, Colour textColour, int textBoxHeight);
    ~SliderContainer();

    void paint (Graphics&) override;
    void resized() override;

private:	
	ScatterSlider scatterSlider;
	ScrambleSlider scrambleSlider;
	RateSlider rateSlider;	
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliderContainer)
};
