/*
  ==============================================================================

    BalanceSlider.cpp
    Created: 1 Jan 2019 2:54:30pm
    Author:  rem_d

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "BalanceSlider.h"

//==============================================================================
BalanceSlider::BalanceSlider(AudioProcessorValueTreeState& valueTreeState, Colour textColour, int textBoxHeight)
{
	balance.setSliderStyle(Slider::LinearHorizontal);
	balance.setRange(0.0, 1.0, 0.01);	
	balance.setValue(0.8);
	balance.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxAbove, false, 100, textBoxHeight);	
	balance.setColour(Slider::ColourIds::textBoxTextColourId, textColour);
	addAndMakeVisible(&balance);
	balanceAttachment.reset(new SliderAttachment(valueTreeState, "balance", balance));

	balanceLabel.setText("Weak/Strong balance", NotificationType::dontSendNotification);
	balanceLabel.attachToComponent(&balance, false);
	balanceLabel.setColour(Label::ColourIds::textColourId, textColour);
	addAndMakeVisible(balanceLabel);
}

BalanceSlider::~BalanceSlider()
{
}

void BalanceSlider::paint (Graphics& g)
{
	g.fillAll(Colours::white);
}

void BalanceSlider::resized()
{
	int y = 0;
	balanceLabel.setBounds(0, y, getWidth() / 3, 30);

	y += balanceLabel.getBounds().getBottom() + 10;;
	balance.setBounds(0, y, getWidth(), getHeight() / 2);
}
