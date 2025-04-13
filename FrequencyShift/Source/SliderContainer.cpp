#include "SliderContainer.h"

SliderContainer::SliderContainer(const std::shared_ptr<FrequencyShiftPluginParameters> &valueTreeState, const Colour textColour, const int textBoxHeight) :
	freqSlider(valueTreeState, textColour, textBoxHeight)
{
    this->pluginParameters = valueTreeState;
	addAndMakeVisible(freqSlider);

	scaleSlider.setSliderStyle(Slider::LinearHorizontal);
	scaleSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxAbove, false, 100, textBoxHeight);
	scaleSlider.setColour(Slider::ColourIds::textBoxTextColourId, textColour);
	scaleSlider.setTooltip("How much to scale the frequencies by");
	scaleAttachment.reset(valueTreeState->createSliderAttachment("scale", scaleSlider));
	addAndMakeVisible(scaleSlider);
}

SliderContainer::~SliderContainer() = default;

void SliderContainer::paint (Graphics& g)
{
	g.fillAll(Colours::white);
}

void SliderContainer::resized()
{
    const int sliderHeight = getHeight() / NUM_VERTICAL_COMPONENTS;
    const int sliderWidth = getWidth();
	int y = 0;
    
    freqSlider.setBounds(0, y, sliderWidth, sliderHeight);
    y = freqSlider.getBottom() + 10;
	scaleSlider.setBounds(0, y, getWidth(), sliderHeight);
}
