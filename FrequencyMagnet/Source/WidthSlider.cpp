#include "WidthSlider.h"

BiasSlider::BiasSlider(std::shared_ptr<PluginParameters> valueTreeState, Colour textColour, int textBoxHeight)
{
	balance.setSliderStyle(Slider::LinearHorizontal);
	balance.setRange(0.0, 1.0, 0.01);
	balance.setValue(0.5);
	balance.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxAbove, false, 100, textBoxHeight);
	balance.setColour(Slider::ColourIds::textBoxTextColourId, textColour);
    balance.setTooltip("How much frequencies will be moved towards the target");
	addAndMakeVisible(&balance);
	balanceAttachment.reset(valueTreeState->createSliderAttachment("width", balance));

	balanceLabel.setText("Width", NotificationType::dontSendNotification);
	balanceLabel.attachToComponent(&balance, false);
	balanceLabel.setColour(Label::ColourIds::textColourId, textColour);
	addAndMakeVisible(balanceLabel); 
}

BiasSlider::~BiasSlider()
{
}

void BiasSlider::paint (Graphics& g)
{
	g.fillAll(Colours::white);
}

void BiasSlider::resized()
{
	int y = 0;
	balanceLabel.setBounds(0, y, getWidth() / 3, 30);

	y += balanceLabel.getBounds().getBottom() + 10;;
	balance.setBounds(0, y, getWidth(), getHeight() / 2);
}
