#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
class CutOffSlider : public Component
{
public:
    CutOffSlider(AudioProcessorValueTreeState& valueTreeState, Colour textColour, int textBoxHeight);
    ~CutOffSlider();

    void paint (Graphics&) override;
    void resized() override;	
private:
	typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;		

	Label cutOffLabel;
	Slider cutOff;	
	std::unique_ptr<SliderAttachment> cutOffAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CutOffSlider)
};
