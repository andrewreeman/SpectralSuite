#include "../JuceLibraryCode/JuceHeader.h"
#include "BiasSlider.h"

//==============================================================================
PhaseSlider::PhaseSlider(AudioProcessorValueTreeState& valueTreeState, Colour textColour, int textBoxHeight)
{
	balance.setSliderStyle(Slider::LinearHorizontal);
	balance.setRange(0.0, 1.0, 0.01);	
	balance.setValue(0.5);
	balance.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxAbove, false, 100, textBoxHeight);	
	balance.setColour(Slider::ColourIds::textBoxTextColourId, textColour);
	addAndMakeVisible(&balance);
	balanceAttachment.reset(new SliderAttachment(valueTreeState, "bias", balance));

	balanceLabel.setText("Width bias", NotificationType::dontSendNotification);
	balanceLabel.attachToComponent(&balance, false);
	balanceLabel.setColour(Label::ColourIds::textColourId, textColour);
	addAndMakeVisible(balanceLabel);
}

PhaseSlider::~PhaseSlider()
{
}

void PhaseSlider::paint (Graphics& g)
{
	g.fillAll(Colours::white);
}

void PhaseSlider::resized()
{
	int y = 0;
	balanceLabel.setBounds(0, y, getWidth() / 3, 30);

	y += balanceLabel.getBounds().getBottom() + 10;;
	balance.setBounds(0, y, getWidth(), getHeight() / 2);
}
