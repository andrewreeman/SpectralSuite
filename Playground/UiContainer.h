#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../shared/ParameterContainerComponent.h"
#include "PlaygroundParameters.h"

class UiContainer : public ParameterContainerComponent
{
public:
    UiContainer(std::shared_ptr<PlaygroundParameters> valueTreeState, Colour textColour, int textBoxHeight);
    ~UiContainer();

    const int getComponentHeight() override { return ParameterContainerComponent::getComponentHeight() * 3; }
    void paint (Graphics&) override;
    void resized() override;
    std::shared_ptr<PluginParameters> getPluginParameters() override { return pluginParameters; }

    Array<PropertyComponent*> getSettingsProperties() override;
    void onPropertiesChanged() override;
    
private:
    Slider mixSlider;
    Label mixLabel;
    std::shared_ptr<PlaygroundParameters> pluginParameters;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachment;
        
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UiContainer)
};
