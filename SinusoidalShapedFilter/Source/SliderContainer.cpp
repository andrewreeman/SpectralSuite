#include "SliderContainer.h"

SliderContainer::SliderContainer(std::shared_ptr<PluginParameters> valueTreeState, Colour textColour, int textBoxHeight) :
	freqSlider(valueTreeState, textColour, textBoxHeight),
	phaseSlider(valueTreeState, textColour, textBoxHeight),
	widthSlider(valueTreeState, textColour, textBoxHeight)
{
    this->valueTreeState = valueTreeState;
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
