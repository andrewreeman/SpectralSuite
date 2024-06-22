#include "SliderContainer.h"

SliderContainer::SliderContainer(std::shared_ptr<FrequencyMagnetParameters> valueTreeState, Colour textColour, int textBoxHeight) :
	freqSlider(valueTreeState, textColour, textBoxHeight),
	phaseSlider(valueTreeState, textColour, textBoxHeight),
	widthSlider(valueTreeState, textColour, textBoxHeight),
	params(valueTreeState)
{
	addAndMakeVisible(freqSlider);
	addAndMakeVisible(phaseSlider);
	addAndMakeVisible(widthSlider);     
}

SliderContainer::~SliderContainer()
{
}

void SliderContainer::paint (Graphics& g)
{
	g.fillAll(Colours::white);
}

void SliderContainer::resized()
{
	int y = 0;
	freqSlider.setBounds(0, y, getWidth(), getHeight() / 3);	

	y += freqSlider.getHeight();
	widthSlider.setBounds(0, y, getWidth(), getHeight() / 3);

	y += widthSlider.getHeight();
	phaseSlider.setBounds(0, y, getWidth(), getHeight() / 3);
}

Array<PropertyComponent*> SliderContainer::getSettingsProperties()
{
	/*const NormalisableRange<float> shiftDownRange = this->valueTreeState->getParameterRange("shiftMinRange");
	SliderPropertyComponent* shiftMinRangeValue = new SliderPropertyComponent(valueTreeState->getParameterAsValue("shiftMinRange"), "Shift down max", shiftDownRange.start, shiftDownRange.end, shiftDownRange.interval, shiftDownRange.skew);

	const NormalisableRange<float> shiftUpRange = this->valueTreeState->getParameterRange("shiftMaxRange");
	SliderPropertyComponent* shiftMaxRangeValue = new SliderPropertyComponent(valueTreeState->getParameterAsValue("shiftMaxRange"), "Shift up max", shiftUpRange.start, shiftUpRange.end, shiftUpRange.interval, shiftUpRange.skew);

	*/
	
	BooleanPropertyComponent* useLegacyHighFrequencyShiftMode = new BooleanPropertyComponent(params->getUseLegacyLogicValue(), "Use bad shift", "");

	Array<PropertyComponent*> settingsPropertyComponents;
	settingsPropertyComponents.add(useLegacyHighFrequencyShiftMode);
    settingsPropertyComponents.addArray(ParameterContainerComponent::getSettingsProperties());
    
	return settingsPropertyComponents;
}
