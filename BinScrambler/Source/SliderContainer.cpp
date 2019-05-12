#include "../JuceLibraryCode/JuceHeader.h"
#include "SliderContainer.h"

SliderContainer::SliderContainer(std::shared_ptr<PluginParameters> valueTreeState, Colour textColour, int textBoxHeight) :	
	scatterSlider(valueTreeState, textColour, textBoxHeight),
	scrambleSlider(valueTreeState, textColour, textBoxHeight),
	rateSlider(valueTreeState, textColour, textBoxHeight)
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
