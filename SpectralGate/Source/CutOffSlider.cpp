#include "../JuceLibraryCode/JuceHeader.h"
#include "CutOffSlider.h"

//==============================================================================
CutOffSlider::CutOffSlider(AudioProcessorValueTreeState& valueTreeState, Colour textColour, int textBoxHeight) 
{
	cutOff.setSliderStyle(Slider::LinearHorizontal);
	cutOff.setRange(0.0, 1.0, 0.01);
	cutOff.setSkewFactor(2.0);
	cutOff.setValue(0.6);
	cutOff.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxAbove, false, 100, textBoxHeight);
	//cutOff.setTextValueSuffix("dB"); // not in db yet!

	cutOff.setColour(Slider::ColourIds::textBoxTextColourId, textColour);	
	addAndMakeVisible(&cutOff);
	cutOffAttachment.reset(new SliderAttachment(valueTreeState, "cutoff", cutOff));	

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
