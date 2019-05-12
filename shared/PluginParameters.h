
#pragma once

#include "JuceHeader.h"

/**
	PluginParameters wraps about an AudioProcessorValueTreeState
**/
class PluginParameters {
public:
	PluginParameters(AudioProcessor* audioProcessor) : valueTreeState(*audioProcessor, nullptr) {};
	
	void createAndAddParameter(std::unique_ptr<RangedAudioParameter> param) {  
		valueTreeState.createAndAddParameter(std::move(param));	
	};
	AudioProcessorValueTreeState::SliderAttachment* createSliderAttachment(const String& parameterID, Slider& sliderToControl) {
		return new AudioProcessorValueTreeState::SliderAttachment(valueTreeState, parameterID, sliderToControl);
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

	float* getRawParameterValue(String paramId) const {
		return valueTreeState.getRawParameterValue(paramId);
	}

	void replaceState(const ValueTree& newState) {
		valueTreeState.replaceState(newState);
	}

	ValueTree copyState() {
		return valueTreeState.copyState();
	}

	ValueTree getState() const {
		return valueTreeState.state;
	}

private:
	AudioProcessorValueTreeState valueTreeState;
};
