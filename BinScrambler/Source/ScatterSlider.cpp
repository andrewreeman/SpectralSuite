#include "ScatterSlider.h"

ScatterSlider::ScatterSlider(std::shared_ptr<PluginParameters> valueTreeState, Colour textColour, int textBoxHeight)
{
	balance.setSliderStyle(Slider::LinearHorizontal);
	balance.setRange(0.0, 1.0, 0.01);	
	balance.setValue(0.8);
	balance.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxAbove, false, 100, textBoxHeight);	
	balance.setColour(Slider::ColourIds::textBoxTextColourId, textColour);
    balance.setTooltip("How far the frequencies are scattered");
	addAndMakeVisible(&balance);
	balanceAttachment.reset(valueTreeState->createSliderAttachment("scatter", balance));

	balanceLabel.setText("Scatter", NotificationType::dontSendNotification);
	balanceLabel.attachToComponent(&balance, false);
	balanceLabel.setColour(Label::ColourIds::textColourId, textColour);
	addAndMakeVisible(balanceLabel);
}

ScatterSlider::~ScatterSlider()
{
}

void ScatterSlider::paint (Graphics& g)
{
	g.fillAll(Colours::white);
}

void ScatterSlider::resized()
{
	int y = 0;
	balanceLabel.setBounds(0, y, getWidth() / 3, 30);

	y += balanceLabel.getBounds().getBottom() + 10;;
	balance.setBounds(0, y, getWidth(), getHeight() / 2);
}
