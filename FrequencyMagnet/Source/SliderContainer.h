#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "FreqSlider.h"
#include "WidthSlider.h"
#include "BiasSlider.h"
#include "../../shared/EditorParameterContainer.h"

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
	FreqSlider freqSlider;
	PhaseSlider phaseSlider;
	BiasSlider widthSlider;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliderContainer)
};
