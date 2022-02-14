#pragma once


#include "../../shared/PluginParameters.h"
#include "../JuceLibraryCode/JuceHeader.h"

class SpectralAudioPlugin;

class PlaygroundParameters : public PluginParameters, public juce::AudioProcessorValueTreeState::Listener {
    static const String SETTINGS_OVERLAP_NAME;

public:
    PlaygroundParameters(SpectralAudioPlugin* processor);
    ~PlaygroundParameters() {
        removeParameterListener(SETTINGS_OVERLAP_NAME, this);
    }
    
    float* getMixParameter() const {
        return getRawParameterValue("mix");
    }
    
    Value getUsePvoc() const {
        return getParameterAsValue("settings_use_pvoc");
    }
    
    Value getDoWindowRotate() const {
        return getParameterAsValue("settings_do_rotate");
    }
    
    Value getThreaded() const {
        return getParameterAsValue("settings_use_threaded_fft");
    }
    
    Value getUseLinearHann() const {
        return getParameterAsValue("settings_use_hann_linear");
    }
    
    Value getNumberOfOverlaps() const {
        return getParameterAsValue(SETTINGS_OVERLAP_NAME);
    }
    
    void parameterChanged(const juce::String &parameterID, float newValue) override;
private:
    SpectralAudioPlugin* m_processor;
};
