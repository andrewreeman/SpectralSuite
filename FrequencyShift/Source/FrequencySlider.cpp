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

	this->valueTreeState = &valueTreeState;

	//AudioParameterFloat* shift = (AudioParameterFloat*)valueTreeState.getParameter("shift");
	//shift->shiftDownRange.start = -600;	
	//frequencyShift.setRange(-600, shiftDownRange.end, shiftDownRange.interval);
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

Array<PropertyComponent*> FrequencySlider::getSettingsProperties() 
{	
	const NormalisableRange<float> shiftDownRange = this->valueTreeState->getParameterRange("shiftMinRange");
	SliderPropertyComponent* shiftMinRangeValue = new SliderPropertyComponent(valueTreeState->getParameterAsValue("shiftMinRange"), "Shift down max", shiftDownRange.start, shiftDownRange.end, shiftDownRange.interval, shiftDownRange.skew);

	const NormalisableRange<float> shiftUpRange = this->valueTreeState->getParameterRange("shiftMaxRange");
	SliderPropertyComponent* shiftMaxRangeValue = new SliderPropertyComponent(valueTreeState->getParameterAsValue("shiftMaxRange"), "Shift up max", shiftUpRange.start, shiftUpRange.end, shiftUpRange.interval, shiftUpRange.skew);

	Array<PropertyComponent*> settingsPropertyComponents;
	settingsPropertyComponents.add(shiftMinRangeValue);
	settingsPropertyComponents.add(shiftMaxRangeValue);

	return settingsPropertyComponents;
}