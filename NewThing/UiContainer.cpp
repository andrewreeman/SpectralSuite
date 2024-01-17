#include "UiContainer.h"

UiContainer::UiContainer(std::shared_ptr<BaseParameters> valueTreeState, Colour textColour, int) :    pluginParameters(valueTreeState)
{
    mixLabel.setText("Mix", NotificationType::dontSendNotification);
    mixLabel.setColour(Label::ColourIds::textColourId, textColour);    
    addAndMakeVisible(&mixLabel);
            
    mixAttachment.reset(valueTreeState->createSliderAttachment("mix", mixSlider));
    mixSlider.setColour(Slider::ColourIds::textBoxTextColourId, textColour);
    addAndMakeVisible(&mixSlider);
    
    mixSlider.setTooltip("Test tooltip");        
}

UiContainer::~UiContainer()
{
}

void UiContainer::paint (Graphics& g)
{
    g.fillAll(Colours::white);
}

void UiContainer::resized()
{
    int x = 16;
    int y = 50;
    
    mixLabel.setBounds(x, y, x + (getWidth() * 0.7), 15);
    y += mixLabel.getBottom() + 8;
    
    mixSlider.setBounds(x, y, x + (getWidth() * 0.7), 30);
}
