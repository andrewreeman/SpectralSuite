#include "SliderContainer.h"

SliderContainer::SliderContainer(AudioProcessorValueTreeState& valueTreeState, Colour textColour, int textBoxHeight) :
	cutOffSlider(valueTreeState, textColour, textBoxHeight),
	balanceSlider(valueTreeState, textColour, textBoxHeight)	
{
	addAndMakeVisible(cutOffSlider);
	addAndMakeVisible(balanceSlider);
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
	cutOffSlider.setBounds(0, y, getWidth(), getHeight() / 2);

	y += cutOffSlider.getBottom();
	balanceSlider.setBounds(0, y, getWidth(), getHeight() / 2);
}
