#include "../JuceLibraryCode/JuceHeader.h"
#include "FrequencySlider.h"

//==============================================================================
FrequencySlider::FrequencySlider(std::shared_ptr<FrequencyShiftPluginParameters> valueTreeState, Colour textColour, int textBoxHeight)
{
	frequencyShift.setSliderStyle(Slider::LinearHorizontal);
		
	//AudioParameterFloat* shift = (AudioParameterFloat*)valueTreeState->getParameter("shift");	
	//const AudioParameterFloat* minRange = (AudioParameterFloat*)valueTreeState->getParameter("shiftMinRange");
	//const AudioParameterFloat* maxRange = (AudioParameterFloat*)valueTreeState->getParameter("shiftMaxRange");
	//	
	//shift->range.start = minRange->get();
	//shift->range.end = maxRange->get();	

	//frequencyShift.setRange(shift->range.start, shift->range.end, shift->range.interval);

	frequencyShift.setSkewFactor(2.0);
	frequencyShift.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxAbove, false, 100, textBoxHeight);		

	frequencyShift.setColour(Slider::ColourIds::textBoxTextColourId, textColour);
	//frequencyShift.setValue(shift->get());
    frequencyShift.setTooltip("How much to shift the frequencies by");
	addAndMakeVisible(&frequencyShift);
	
	frequencyShiftAttachment.reset(valueTreeState->createSliderAttachment("shift", frequencyShift));//  new SliderAttachment(valueTreeState, "shift", frequencyShift));

	this->valueTreeState = valueTreeState;
	//this->onPropertiesChanged();

	//AudioParameterFloat* shift = (AudioParameterFloat*)valueTreeState.getParameter("shift");
	//shift->shiftDownRange.start = -600;	
	//frequencyShift.setRange(-600, shiftDownRange.end, shiftDownRange.interval);
}

FrequencySlider::~FrequencySlider()
{
}

void FrequencySlider::paint(Graphics& g)
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
	this->valueTreeState->updateValue(&frequencyShift, shiftParam->get());	
}

void FrequencySlider::onAudioValueTreeStateLoadedFromXmlState(PluginParameters*, XmlElement* xmlState)
{				
	AudioParameterFloat* shiftParam = (AudioParameterFloat*)this->valueTreeState->getParameter("shift");	
	const double originalShiftValue = xmlState->getChildByAttribute("id", "shift")->getDoubleAttribute("value", shiftParam->get());		
	this->valueTreeState->updateValue(&frequencyShift, originalShiftValue);	
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
    
    PropertyComponent* fftStyle = this->fftStylePropertyComponent(this->valueTreeState);
    settingsPropertyComponents.add(fftStyle);

	return settingsPropertyComponents;
}
