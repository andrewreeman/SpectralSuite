#include "SliderContainer.h"

SliderContainer::SliderContainer(std::shared_ptr<PluginParameters> valueTreeState, Colour textColour, int textBoxHeight) :
	cutOffSlider(valueTreeState, textColour, textBoxHeight),
	balanceSlider(valueTreeState, textColour, textBoxHeight),
    tiltSlider(valueTreeState, textColour, textBoxHeight),
    tiltToggle("Enable tilt")
{
    this->pluginParameters = valueTreeState;
	addAndMakeVisible(cutOffSlider);
	addAndMakeVisible(balanceSlider);
    
    tiltToggle.setButtonText("Enable tilt");
    tiltToggle.setToggleState(false, NotificationType::dontSendNotification);
    tiltToggle.setColour(ToggleButton::ColourIds::textColourId, textColour);
    tiltToggle.setColour(ToggleButton::ColourIds::tickDisabledColourId, textColour);
    tiltToggle.setColour(ToggleButton::ColourIds::tickColourId, textColour);
    tiltToggle.addListener(this);
    addAndMakeVisible(tiltToggle);
    
    addAndMakeVisible(tiltSlider);
}

SliderContainer::~SliderContainer()
{
}


void SliderContainer::buttonClicked (Button* clickedButton) {
    if(clickedButton != &tiltToggle) { return; }
    
    auto param = dynamic_cast<AudioParameterBool*>(this->pluginParameters->getParameter("enableTilt"));
    if(param == nullptr) { return; }
    
    *param = tiltToggle.getToggleState();
}

void SliderContainer::paint (Graphics& g)
{
	g.fillAll(Colours::white);
}

void SliderContainer::resized()
{
    int sliderHeight = getHeight() / SliderContainer::NUM_VERTICAL_COMPONENTS;
    int sliderWidth = getWidth();
	int y = 0;
    
    cutOffSlider.setBounds(0, y, sliderWidth, sliderHeight);

    y += cutOffSlider.getHeight();
    balanceSlider.setBounds(0, y, sliderWidth, sliderHeight);
    
    y += balanceSlider.getHeight();
    tiltToggle.setBounds(0, y, 50, 50);
    
    y += tiltToggle.getHeight();
    
    tiltSlider.setBounds(0, y, sliderWidth, sliderHeight);
}
