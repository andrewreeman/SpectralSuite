#include "../JuceLibraryCode/JuceHeader.h"
#include "ScatterAmountSlider.h"

//==============================================================================
PhaseSlider::PhaseSlider(AudioProcessorValueTreeState& valueTreeState, Colour textColour, int textBoxHeight)
{
	balance.setSliderStyle(Slider::LinearHorizontal);
	balance.setRange(0.0, 1.0, 0.01);	
	balance.setValue(0.8);
	balance.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxAbove, false, 100, textBoxHeight);	
	balance.setColour(Slider::ColourIds::textBoxTextColourId, textColour);
	addAndMakeVisible(&balance);
	balanceAttachment.reset(new SliderAttachment(valueTreeState, "phase", balance));

	balanceLabel.setText("Phase", NotificationType::dontSendNotification);
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
