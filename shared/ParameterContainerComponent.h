#pragma once

#include "JuceHeader.h"
#include "SettingsPage.h"
#include "AudioValueTreeOnLoadListener.h"
#include "PluginParameters.h"

//==============================================================================
/*
*/
class ParameterContainerComponent : 
	public Component,
	public SettingsPage::Listener,
	public AudioValueTreeStateOnLoadListener	
{
public:
    ParameterContainerComponent();
    ~ParameterContainerComponent();

	virtual const int getComponentHeight() { return 80; }
    virtual std::shared_ptr<PluginParameters> getPluginParameters() = 0;
	virtual void paint(Graphics&) override = 0;
	virtual void resized() override = 0;

	virtual Array<PropertyComponent*> getSettingsProperties() {
		auto settings = Array<PropertyComponent*>();
        auto pluginParameters = getPluginParameters();
        
        settings.add(fftStylePropertyComponent(pluginParameters));
        settings.add(fftOverlapsPropertyComponent(pluginParameters));
        settings.add(fftWindowPropertyComponent(pluginParameters));
        
        return settings;
	}
    
    virtual void onFftSizeChanged() {}
    virtual void onFftStyleChanged() {}

	// Inherited via Listener
	virtual void onPropertiesChanged() override {}

	// Inherited via AudioValueTreeStateOnLoadListener
	virtual void onAudioValueTreeStateLoadedFromXmlState(PluginParameters*, XmlElement*) override {}
    
protected:
    PropertyComponent* fftStylePropertyComponent(std::shared_ptr<PluginParameters> valueTreeState) {
        StringArray choices;
        choices.add("Default");
        choices.add("PVOC");
        
        Array<var> values;
        values.add(0);
        values.add(1);
        
        String fftKey = "fftStyle";
        return new ChoicePropertyComponent(valueTreeState->getParameterAsValue(fftKey), "FFT Style", choices, values);
    }
    
    PropertyComponent* fftOverlapsPropertyComponent(std::shared_ptr<PluginParameters> valueTreeState) {
        String fftKey = "fftOverlaps";
        AudioParameterChoice* param = valueTreeState->getParameterChoice(fftKey);
        StringArray choices = param->choices;
        
        Array<var> values;
        for(int i=0; i<choices.size(); ++i) {
            values.add(i);
        }
        
        return new ChoicePropertyComponent(valueTreeState->getParameterAsValue(fftKey), "FFT Overlap count", choices, values);
    }
    
    PropertyComponent* fftWindowPropertyComponent(std::shared_ptr<PluginParameters> valueTreeState) {
        String fftKey = "fftWindow";
        AudioParameterChoice* param = valueTreeState->getParameterChoice(fftKey);
        StringArray choices = param->choices;
        
        Array<var> values;
        for(int i=0; i<choices.size(); ++i) {
            values.add(i);
        }
        
        return new ChoicePropertyComponent(valueTreeState->getParameterAsValue(fftKey), "FFT window type", choices, values);
    }
    
    
private:
    // This just needs to be declared: https://forum.juce.com/t/no-button-tooltips-in-juce-widgets-demo/36479/5
    // for showing tooltips
    SharedResourcePointer<TooltipWindow> tooltipWindow;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParameterContainerComponent)	
};
