#include "TiltSlider.h"

TiltSlider::TiltSlider(std::shared_ptr<PluginParameters> valueTreeState, Colour textColour, int textBoxHeight)
{
    tilt.setSliderStyle(Slider::LinearHorizontal);
    tilt.setRange(0.0, 1.0, 0.01);
    tilt.setSkewFactor(2.0);
    tilt.setValue(0.5);
    tilt.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxAbove, false, 100, textBoxHeight);
    tilt.setColour(Slider::ColourIds::textBoxTextColourId, textColour);
    tilt.setTooltip("Direction gate is tilted, biasing towards lower or higher frequencies");
    addAndMakeVisible(&tilt);

    tiltAttachment.reset(valueTreeState->createSliderAttachment("tilt", tilt));
    tiltLabel.setText("Gate tilt", NotificationType::dontSendNotification);
    tiltLabel.attachToComponent(&tilt, false);
    tiltLabel.setColour(Label::ColourIds::textColourId, textColour);
    addAndMakeVisible(tiltLabel);
}

TiltSlider::~TiltSlider()
{
}

void TiltSlider::paint (Graphics& g)
{
    g.fillAll(Colours::white);
}

void TiltSlider::resized()
{
    int y = 0;
    tiltLabel.setBounds(0, y, getWidth() / 3, 30);
    
    y += tiltLabel.getBounds().getBottom() + 10;;
    tilt.setBounds(0, y, getWidth(), getHeight() / 2);
}

