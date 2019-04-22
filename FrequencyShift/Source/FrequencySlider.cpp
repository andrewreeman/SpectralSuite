#include "../JuceLibraryCode/JuceHeader.h"
#include "FrequencySlider.h"

//==============================================================================
FrequencySlider::FrequencySlider(AudioProcessorValueTreeState& valueTreeState, Colour textColour, int textBoxHeight) 
{
	frequencyShift.setSliderStyle(Slider::LinearHorizontal);
	/*AudioParameterFloat* shift = (AudioParameterFloat*)valueTreeState.getParameter("shift");	
	const AudioParameterFloat* minRange = (AudioParameterFloat*)valueTreeState.getParameter("shiftMinRange");
	const AudioParameterFloat* maxRange = (AudioParameterFloat*)valueTreeState.getParameter("shiftMaxRange");
		
	shift->range.start = minRange->get();
	shift->range.end = maxRange->get();	*/

	//frequencyShift.setRange(range.start, range.end, range.interval);
	frequencyShift.setSkewFactor(2.0);
	frequencyShift.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxAbove, false, 100, textBoxHeight);		

	frequencyShift.setColour(Slider::ColourIds::textBoxTextColourId, textColour);
	//frequencyShift.setValue(shift->get());
	addAndMakeVisible(&frequencyShift);
	
	frequencyShiftAttachment.reset(new SliderAttachment(valueTreeState, "shift", frequencyShift));	

	this->valueTreeState = &valueTreeState;
	this->onPropertiesChanged();

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
	AudioParameterFloat* shiftParam = (AudioParameterFloat*)this->valueTreeState->getParameter("shift");
	const AudioParameterFloat* lowestValueParam = (AudioParameterFloat*)this->valueTreeState->getParameter("shiftMinRange");
	const AudioParameterFloat* highestValueParam = (AudioParameterFloat*)this->valueTreeState->getParameter("shiftMaxRange");	
	
	float currentValue = shiftParam->get();
	const float lowestValue = lowestValueParam->get();
	const float highestValue = highestValueParam->get();

	if (currentValue < lowestValue) {
		currentValue = lowestValue;
	}

	if (currentValue > highestValue) {
		currentValue = highestValue;
	}

	shiftParam->range.start = lowestValue;
	shiftParam->range.end = highestValue;	
	shiftParam->setValueNotifyingHost(shiftParam->convertTo0to1(currentValue));

	frequencyShift.setRange(shiftParam->range.start, shiftParam->range.end, shiftParam->range.interval);

	
	

	/*shift->shiftDownRange.start = -600;	
	frequencyShift.setRange(-600, shiftDownRange.end, shiftDownRange.interval);*/

}

void FrequencySlider::onAudioValueTreeStateLoadedFromXmlState(AudioProcessorValueTreeState & newState, XmlElement* xmlState)
{				
	AudioParameterFloat* shiftParam = (AudioParameterFloat*)this->valueTreeState->getParameter("shift");
	
	const double originalShiftValue = xmlState->getChildByAttribute("id", "shift")->getDoubleAttribute("value", shiftParam->get());	
	const AudioParameterFloat* lowestValueParam = (AudioParameterFloat*)this->valueTreeState->getParameter("shiftMinRange");
	const AudioParameterFloat* highestValueParam = (AudioParameterFloat*)this->valueTreeState->getParameter("shiftMaxRange");

	float currentValue = originalShiftValue;
	const float lowestValue = lowestValueParam->get();
	const float highestValue = highestValueParam->get();

	if (currentValue < lowestValue) {
		currentValue = lowestValue;
	}

	if (currentValue > highestValue) {
		currentValue = highestValue;
	}

	shiftParam->range.start = lowestValue;
	shiftParam->range.end = highestValue;	
	shiftParam->setValueNotifyingHost(shiftParam->convertTo0to1(currentValue));	
	
	frequencyShift.setRange(shiftParam->range.start, shiftParam->range.end, shiftParam->range.interval);
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