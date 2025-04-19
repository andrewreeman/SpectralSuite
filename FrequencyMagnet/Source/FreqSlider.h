#pragma once

#include "JuceHeader.h"
#include "../../shared/PluginParameters.h"

class FreqSlider : public Component
{
public:
    FreqSlider(std::shared_ptr<PluginParameters> valueTreeState, Colour textColour, int textBoxHeight);
    ~FreqSlider() override;

    void paint (Graphics&) override;
    void resized() override;	
    void setRange (double newMin, double newMax, double newInt);
private:
	typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;		

    Label pitchLabel;
	Label freqLabel;
	Slider freq;	
	std::unique_ptr<SliderAttachment> freqAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FreqSlider)
};
