#include "SliderContainer.h"

SliderContainer::SliderContainer(std::shared_ptr<PluginParameters> valueTreeState, Colour textColour, int textBoxHeight) :
	cutOffSlider(valueTreeState, textColour, textBoxHeight),
	balanceSlider(valueTreeState, textColour, textBoxHeight),
    tiltSlider(valueTreeState, textColour, textBoxHeight)
{
    this->pluginParameters = valueTreeState;
	addAndMakeVisible(cutOffSlider);
	addAndMakeVisible(balanceSlider);
    addAndMakeVisible(tiltSlider);
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
    int sliderHeight = getHeight() / SliderContainer::NUM_SLIDERS;
    int sliderWidth = getWidth();
	int y = 0;
    
    cutOffSlider.setBounds(0, y, sliderWidth, sliderHeight);

    y += cutOffSlider.getHeight();
    balanceSlider.setBounds(0, y, sliderWidth, sliderHeight);
    
    y += balanceSlider.getHeight();
    tiltSlider.setBounds(0, y, sliderWidth, sliderHeight);
}
