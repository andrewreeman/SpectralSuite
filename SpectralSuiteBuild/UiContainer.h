#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../shared/ParameterContainerComponent.h"
#include "BaseParameters.h"

class UiContainer : public ParameterContainerComponent
{
public:
    UiContainer(std::shared_ptr<BaseParameters> valueTreeState, Colour textColour, int textBoxHeight);
    ~UiContainer();

    std::shared_ptr<PluginParameters> getPluginParameters() override { return pluginParameters; }
    const int getComponentHeight() override { return ParameterContainerComponent::getComponentHeight() * 3; }
    void paint (Graphics&) override;
    void resized() override;    
    
private:
    Slider mixSlider;
    Label mixLabel;
    std::shared_ptr<BaseParameters> pluginParameters;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachment;        
        
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UiContainer)
};
