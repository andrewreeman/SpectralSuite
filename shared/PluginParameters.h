
#pragma once

#include "JuceHeader.h"

/**
	PluginParameters wraps about an AudioProcessorValueTreeState
**/
class PluginParameters {
public:
	PluginParameters(AudioProcessor* audioProcessor) : valueTreeState(*audioProcessor, nullptr) {};
    
    virtual ~PluginParameters(){};
	
    virtual void replaceState(const ValueTree& newState) {
        valueTreeState.replaceState(newState);
    }

    virtual ValueTree copyState() {
        return valueTreeState.copyState();
    }
    
	void createAndAddParameter(std::unique_ptr<RangedAudioParameter> param) {
		valueTreeState.createAndAddParameter(std::move(param));	
	};

	AudioProcessorValueTreeState::SliderAttachment* createSliderAttachment(const String& parameterID, Slider& sliderToControl) {
		return new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, parameterID, sliderToControl);
	};

	AudioProcessorValueTreeState::ButtonAttachment* createButtonAttachment(const String& parameterID, Button& buttonToControl) {
		return new AudioProcessorValueTreeState::ButtonAttachment(valueTreeState, parameterID, buttonToControl);
	};

	AudioProcessorValueTreeState::ComboBoxAttachment* createComboBoxAttachment(const String& parameterID, ComboBox& comboBoxToControl) {
		return new AudioProcessorValueTreeState::ComboBoxAttachment(valueTreeState, parameterID, comboBoxToControl);
	};
	
	RangedAudioParameter* getParameter(String paramId) const {
		return valueTreeState.getParameter(paramId);
	}

	NormalisableRange<float> getParameterRange(StringRef paramID) const {
		return valueTreeState.getParameterRange(paramID);
	}

	Value getParameterAsValue(String paramId) const {
		return valueTreeState.getParameterAsValue(paramId);
	}
 
    void addParameterListener (StringRef paramID, juce::AudioProcessorValueTreeState::Listener* listener) {
        valueTreeState.addParameterListener(paramID, listener);
    }
    
    void removeParameterListener (StringRef parameterID, juce::AudioProcessorValueTreeState::Listener* listener) {
        valueTreeState.removeParameterListener(parameterID, listener);
    }

	float* getRawParameterValue(String paramId) const {
        return (float*)valueTreeState.getRawParameterValue(paramId);
	}

	ValueTree getState() const {
		return valueTreeState.state;
	}
    
    UndoManager* getUndoManager() const {
        return valueTreeState.undoManager;
    }
    
    

private:
	AudioProcessorValueTreeState valueTreeState;
};
