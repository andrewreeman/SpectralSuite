#include "UiContainer.h"

UiContainer::UiContainer(std::shared_ptr<PlaygroundParameters> valueTreeState, Colour textColour, int) :    pluginParameters(valueTreeState)
{
    mixLabel.setText("Mix", NotificationType::dontSendNotification);
    mixLabel.attachToComponent(&mixSlider, false);
    mixLabel.setColour(Label::ColourIds::textColourId, textColour);
    
    mixAttachment.reset(valueTreeState->createSliderAttachment("mix", mixSlider));
    mixSlider.setColour(Slider::ColourIds::textBoxTextColourId, textColour);
    addAndMakeVisible(&mixSlider);
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
    
    mixSlider.setBounds(x, y, x + (getWidth() * 0.7), 30);
}

Array<PropertyComponent*> UiContainer::getSettingsProperties() {
    Array<PropertyComponent*> properties;
    
    properties.add(new BooleanPropertyComponent(pluginParameters->getUsePvoc(), "Use Phase Vocoder",""));
    properties.add(new BooleanPropertyComponent(pluginParameters->getDoWindowRotate(), "PVoc: Window rotate",""));
    properties.add(new BooleanPropertyComponent(pluginParameters->getThreaded(), "PVoc: Use thread for large FFT",""));
    properties.add(new BooleanPropertyComponent(pluginParameters->getUseLinearHann(), "PVoc: Use linear hann",""));
    
    
    auto overlapChoiceComponent = new ChoicePropertyComponent(
        pluginParameters->getNumberOfOverlaps(),
        "Number of overlaps",
        StringArray("1", "2", "4", "8"),
        Array<var>(1, 2, 4, 8)
    );    
    
    
    properties.add(overlapChoiceComponent);
    
    return properties;
}

void UiContainer::onPropertiesChanged() {
     auto overlaps = (int)pluginParameters->getNumberOfOverlaps().getValue();
     int a = 4;
}
