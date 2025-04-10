#include "SliderContainer.h"

SliderContainer::SliderContainer(std::shared_ptr<FrequencyShiftPluginParameters> valueTreeState, Colour textColour, int textBoxHeight) :
	freqSlider(valueTreeState, textColour, textBoxHeight)
{
    this->pluginParameters = valueTreeState;
	addAndMakeVisible(freqSlider);
}

SliderContainer::~SliderContainer() {
}

void SliderContainer::paint (Graphics& g)
{
	g.fillAll(Colours::white);
}

void SliderContainer::resized()
{
    int sliderHeight = getHeight() / SliderContainer::NUM_VERTICAL_COMPONENTS;
    int sliderWidth = getWidth();
	int y = 0;
    
    freqSlider.setBounds(0, y, sliderWidth, sliderHeight);
}
