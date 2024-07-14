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
    addAndMakeVisible(tiltToggle);
    m_tiltButtonAttachment.reset(valueTreeState->createButtonAttachment(juce::String{ "enableTilt" }, tiltToggle));
    
    addAndMakeVisible(tiltSlider);
}

SliderContainer::~SliderContainer() {
    m_tiltButtonAttachment = nullptr;
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
    tiltToggle.setBounds(0, y, 100, 50);
    
    y += tiltToggle.getHeight();
    
    tiltSlider.setBounds(0, y, sliderWidth, sliderHeight);
}
