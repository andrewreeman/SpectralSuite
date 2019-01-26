#include "../JuceLibraryCode/JuceHeader.h"
#include "FreqSlider.h"

//==============================================================================
FreqSlider::FreqSlider(AudioProcessorValueTreeState& valueTreeState, Colour textColour, int textBoxHeight) 
{
	auto freqRange = valueTreeState.getParameterRange("freq");	
	auto freqDefaultValue = valueTreeState.getRawParameterValue("freq");

	freq.setSliderStyle(Slider::LinearHorizontal);
	freq.setRange(freqRange.start , freqRange.end, 10.0);	
	freq.setValue(*freqDefaultValue, NotificationType::dontSendNotification);	
	freq.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxAbove, false, 100, textBoxHeight);
	freq.setTextValueSuffix("Hz");
	
	freq.setColour(Slider::ColourIds::textBoxTextColourId, textColour);	
	addAndMakeVisible(&freq);
	freqAttachment.reset(new SliderAttachment(valueTreeState, "freq", freq));	

	freqLabel.setText("Frequency", NotificationType::dontSendNotification);
	freqLabel.attachToComponent(&freq, false);
	freqLabel.setColour(Label::ColourIds::textColourId, textColour);
	addAndMakeVisible(freqLabel);
}

FreqSlider::~FreqSlider()
{
}

void FreqSlider::paint (Graphics& g)
{
	g.fillAll(Colours::white); 
}

void FreqSlider::resized()
{    
	int y = 0;
	freqLabel.setBounds(0, y, getWidth() / 3, 30);
	
	y += freqLabel.getBounds().getBottom() + 10;;
	freq.setBounds(0, y, getWidth(), getHeight() / 2);
}
