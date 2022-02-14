#include "CutOffSlider.h"

CutOffSlider::CutOffSlider(std::shared_ptr<PluginParameters> valueTreeState, Colour textColour, int textBoxHeight) 
{
	cutOff.setSliderStyle(Slider::LinearHorizontal);
	cutOff.setRange(0.0, 1.0, 0.01);	
	cutOff.setSkewFactor(2.0);
	cutOff.setValue(0.6);
	cutOff.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxAbove, false, 100, textBoxHeight);	
	cutOff.setColour(Slider::ColourIds::textBoxTextColourId, textColour);
    cutOff.setTooltip("Frequency above or below cut-off will be allowed, according to the balance");
	addAndMakeVisible(&cutOff);

	cutOffAttachment.reset(valueTreeState->createSliderAttachment("cutoff", cutOff));	
	cutOffLabel.setText("Cutoff amplitude", NotificationType::dontSendNotification);
	cutOffLabel.attachToComponent(&cutOff, false);
	cutOffLabel.setColour(Label::ColourIds::textColourId, textColour);
	addAndMakeVisible(cutOffLabel);
}

CutOffSlider::~CutOffSlider()
{
}

void CutOffSlider::paint (Graphics& g)
{
	g.fillAll(Colours::white); 
}

void CutOffSlider::resized()
{    
	int y = 0;
	cutOffLabel.setBounds(0, y, getWidth() / 3, 30);
	
	y += cutOffLabel.getBounds().getBottom() + 10;;
	cutOff.setBounds(0, y, getWidth(), getHeight() / 2);
}
