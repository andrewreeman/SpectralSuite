#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../shared/ParameterContainerComponent.h"
#include "ControlPointComponent.h"
#include "PhaseLockParameters.h"

class SliderContainer : public ParameterContainerComponent, ControlPointComponent::Listener
{
public:
	SliderContainer(std::shared_ptr<PhaseLockParameters> valueTreeState, Colour textColour, int textBoxHeight);
    ~SliderContainer();

    std::shared_ptr<PluginParameters> getPluginParameters() override { return m_pluginParameters; }
	const int getComponentHeight() override { return ParameterContainerComponent::getComponentHeight() * 5; }
    void paint (Graphics&) override;
    void resized() override;
    
    void controlPointsChanged(Array<float> outputValues, ControlPointComponent* component) override;
    void onAudioValueTreeStateLoadedFromXmlState(PluginParameters*, XmlElement*) override;

private:	
	using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

	ToggleButton m_lockPhase;
    Slider m_phaseMixSlider;
    Label m_phaseMixLabel;
    
    ToggleButton m_lockMag;
    Slider m_magMixSlider;
    Label m_magMixLabel;
    
    Slider m_magTrackSlider;
    Label m_magTrackLabel;
    
    Slider m_randomPhaseSlider;
    Label m_randomPhaseLabel;
    
    ToggleButton m_startMorphPhaseAndMag;
    Slider m_morphDurationSeconds;
    
	std::shared_ptr<PhaseLockParameters> m_pluginParameters;
	std::unique_ptr<ButtonAttachment> m_phaseLockAttachment;
    std::unique_ptr<ButtonAttachment> m_magLockAttachment;
        
    std::unique_ptr<SliderAttachment> m_phaseMixAttachment;
    std::unique_ptr<SliderAttachment> m_magMixAttachment;
    std::unique_ptr<SliderAttachment> m_magTrackAttachment;
    std::unique_ptr<SliderAttachment> m_randomPhaseAttachment;
    
    std::unique_ptr<ButtonAttachment> m_morphMagAndPhaseAttachment;
    std::unique_ptr<SliderAttachment> m_morphDurationSecondsAttachment;
		
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliderContainer)		
};
