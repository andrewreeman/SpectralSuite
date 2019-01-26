#include "../JuceLibraryCode/JuceHeader.h"
#include "FrequencySlider.h"

//==============================================================================
FrequencySlider::FrequencySlider(AudioProcessorValueTreeState& valueTreeState, Colour textColour, int textBoxHeight) 
{
	frequencyShift.setSliderStyle(Slider::LinearHorizontal);
	frequencyShift.setRange(-500, 500, 10);
	frequencyShift.setSkewFactor(2.0);
	frequencyShift.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxAbove, false, 100, textBoxHeight);
	frequencyShift.setTextValueSuffix("Hz");

	frequencyShift.setColour(Slider::ColourIds::textBoxTextColourId, textColour);
	frequencyShift.setValue(0.6);
	addAndMakeVisible(&frequencyShift);
	frequencyShiftAttachment.reset(new SliderAttachment(valueTreeState, "shift", frequencyShift));	
}

FrequencySlider::~FrequencySlider()
{
}

void FrequencySlider::paint (Graphics& g)
{
	g.fillAll(Colours::white); 
}

void FrequencySlider::resized()
{    
	frequencyShift.setBounds(0, 0, getWidth(), getHeight());
}
