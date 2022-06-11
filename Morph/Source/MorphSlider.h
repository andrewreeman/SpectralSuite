#pragma once

#include "JuceHeader.h"
#include "MorphPluginParameters.h"
#include "../../shared/SettingsPage.h"
#include "../../shared/ParameterContainerComponent.h"
#include "../../shared/components/ControlPointComponent.h"

//==============================================================================
class MorphSlider : public ParameterContainerComponent	
{
public:
    MorphSlider(std::shared_ptr<MorphPluginParameters> valueTreeState, Colour textColour, int textBoxHeight);
    ~MorphSlider();

    void paint (Graphics&) override;
    void resized() override;	
					
    std::shared_ptr<PluginParameters> getPluginParameters() override { return valueTreeState; }
    const int getComponentHeight() override { return 120; }
    void onFftSizeChanged() override {
        controlPointComponent.notifyChanged();
    }

private:
	std::shared_ptr<MorphPluginParameters> valueTreeState;
    ControlPointComponent controlPointComponent;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MorphSlider)	
};
