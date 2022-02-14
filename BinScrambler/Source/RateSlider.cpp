#include "RateSlider.h"

RateSlider::RateSlider(std::shared_ptr<PluginParameters> valueTreeState, Colour textColour, int textBoxHeight)
{
	rate.setSliderStyle(Slider::LinearHorizontal);

	auto param = valueTreeState->getParameter("rate");
	rate.setRange(param->getNormalisableRange().start, param->getNormalisableRange().end, 0.1);
	rate.setValue(param->getValue(), NotificationType::dontSendNotification);
	rate.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxAbove, false, 100, textBoxHeight);	
	rate.setColour(Slider::ColourIds::textBoxTextColourId, textColour);
    rate.setTooltip("How fast frequencies are scrambled");
	addAndMakeVisible(&rate);
	rateAttachment.reset(valueTreeState->createSliderAttachment("rate", rate));

	rateLabel.setText("Rate", NotificationType::dontSendNotification);
	rateLabel.attachToComponent(&rate, false);
	rateLabel.setColour(Label::ColourIds::textColourId, textColour);
	addAndMakeVisible(rateLabel);
}

RateSlider::~RateSlider()
{
}

void RateSlider::paint(Graphics& g)
{
	g.fillAll(Colours::white);
}

void RateSlider::resized()
{
	int y = 0;
	rateLabel.setBounds(0, y, getWidth() / 3, 30);

	y += rateLabel.getBounds().getBottom() + 10;;
	rate.setBounds(0, y, getWidth(), getHeight() / 2);
}
