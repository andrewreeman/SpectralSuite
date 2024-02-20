#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../shared/ParameterContainerComponent.h"
#include "CutOffSlider.h"
#include "BalanceSlider.h"
#include "TiltSlider.h"

//==============================================================================
/*
*/
class SliderContainer : public ParameterContainerComponent
{
public:
	SliderContainer(std::shared_ptr<PluginParameters> valueTreeState, Colour textColour, int textBoxHeight);
    ~SliderContainer();
    
    std::shared_ptr<PluginParameters> getPluginParameters() override {
        return pluginParameters;
    }

	const int getComponentHeight() override { return ParameterContainerComponent::getComponentHeight() * SliderContainer::NUM_VERTICAL_COMPONENTS; }
    void paint (Graphics&) override;
    void resized() override;

private:
    static int const NUM_VERTICAL_COMPONENTS = 4;
    std::shared_ptr<PluginParameters> pluginParameters;
	CutOffSlider cutOffSlider;
	BalanceSlider balanceSlider;
    TiltSlider tiltSlider;
    ToggleButton tiltToggle;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> m_tiltButtonAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliderContainer)
};
