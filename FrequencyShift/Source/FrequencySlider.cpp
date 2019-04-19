#include "../JuceLibraryCode/JuceHeader.h"
#include "FrequencySlider.h"

//==============================================================================
FrequencySlider::FrequencySlider(AudioProcessorValueTreeState& valueTreeState, Colour textColour, int textBoxHeight) 
{
	frequencyShift.setSliderStyle(Slider::LinearHorizontal);
	const NormalisableRange<float> range = valueTreeState.getParameterRange("shift");		
		
	frequencyShift.setRange(range.start, range.end, range.interval);
	frequencyShift.setSkewFactor(2.0);
	frequencyShift.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxAbove, false, 100, textBoxHeight);		

	frequencyShift.setColour(Slider::ColourIds::textBoxTextColourId, textColour);
	frequencyShift.setValue(0.6);
	addAndMakeVisible(&frequencyShift);
	
	frequencyShiftAttachment.reset(new SliderAttachment(valueTreeState, "shift", frequencyShift));	

	//AudioParameterFloat* shift = (AudioParameterFloat*)valueTreeState.getParameter("shift");
	//shift->range.start = -600;	
	//frequencyShift.setRange(-600, range.end, range.interval);
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

void FrequencySlider::onPropertiesChanged()
{
}
