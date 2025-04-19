#include "ScaleSlider.h"

ScaleSlider::ScaleSlider(std::shared_ptr<FrequencyShiftPluginParameters> valueTreeState, Colour textColour, int textBoxHeight)
{
	scaleSlider.setSliderStyle(Slider::LinearHorizontal);
	scaleSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxAbove, false, 100, textBoxHeight);
	scaleSlider.setColour(Slider::ColourIds::textBoxTextColourId, textColour);
	scaleSlider.setTooltip("How much to scale the frequencies by");
	scaleAttachment.reset(valueTreeState->createSliderAttachment("scale", scaleSlider));
	addAndMakeVisible(scaleSlider);

	scaleLabel.setText("Scale", NotificationType::dontSendNotification);
	scaleLabel.attachToComponent(&scaleSlider, false);
	scaleLabel.setColour(Label::ColourIds::textColourId, textColour);
}

ScaleSlider::~ScaleSlider()= default;

void ScaleSlider::paint(Graphics& g)
{
	g.fillAll(Colours::white);
}

void ScaleSlider::resized()
{
	int y = 0;
	scaleLabel.setBounds(0, y, getWidth() / 3, 30);
	y += scaleLabel.getBounds().getBottom() + 10;
	scaleSlider.setBounds(0, y, getWidth(), static_cast<int>(getHeight() / 2));
}
