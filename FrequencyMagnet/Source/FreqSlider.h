#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
class FreqSlider : public Component
{
public:
    FreqSlider(AudioProcessorValueTreeState& valueTreeState, Colour textColour, int textBoxHeight);
    ~FreqSlider();

    void paint (Graphics&) override;
    void resized() override;	
private:
	typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;		

	Label freqLabel;
	Slider freq;	
	std::unique_ptr<SliderAttachment> freqAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FreqSlider)
};
