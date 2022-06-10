#include "SliderContainer.h"

SliderContainer::SliderContainer(std::shared_ptr<BinScramblerParameters> valueTreeState, Colour textColour, int textBoxHeight) :	
	scatterSlider(valueTreeState, textColour, textBoxHeight),
	scrambleSlider(valueTreeState, textColour, textBoxHeight),
	rateSlider(valueTreeState, textColour, textBoxHeight),
	pluginParameters(valueTreeState)
{    
	addAndMakeVisible(scatterSlider);
	addAndMakeVisible(scrambleSlider);
	addAndMakeVisible(rateSlider);
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
	scrambleSlider.setBounds(0, y, getWidth(), getHeight() / 3);

	y += scrambleSlider.getHeight();
	scatterSlider.setBounds(0, y, getWidth(), getHeight() / 3);

	y += scatterSlider.getHeight();
	rateSlider.setBounds(0, y, getWidth(), getHeight() / 3);
}

Array<PropertyComponent*> SliderContainer::getSettingsProperties()
{
	Array<PropertyComponent*> propertyComponents;

	AudioParameterInt* seedIntParameter = this->pluginParameters->getSeedParameter();
	Range<int> range = seedIntParameter->getRange();
	
	SliderPropertyComponent* shiftMaxRangeValue = new SliderPropertyComponent(this->pluginParameters->getSeedParameterAsValue(), "Random seed (0 for random)", (double)range.getStart(), (double)range.getEnd(), 1.0);
    
    
	propertyComponents.add(shiftMaxRangeValue);
    
    PropertyComponent* fftStyle = this->fftStylePropertyComponent(this->pluginParameters);
    propertyComponents.add(fftStyle);

	return propertyComponents;
}
