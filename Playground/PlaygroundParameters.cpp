#include "PlaygroundParameters.h"
#include "../shared/SpectralAudioPlugin.h"

const String PlaygroundParameters::SETTINGS_OVERLAP_NAME = "settings_overlap_count";

PlaygroundParameters::PlaygroundParameters(SpectralAudioPlugin * processor) :
    PluginParameters(processor), m_processor(processor)
{
    createAndAddParameter(std::make_unique<AudioParameterFloat>(
                                                                ParameterID("mix", 1),
                                                                "Generic mix parameter",
                                                                0.0,
                                                                1.0,
                                                                1.0
                                                                ));
    
    createAndAddParameter(std::make_unique<AudioParameterBool>(ParameterID("settings_use_pvoc", 1), "Use Phase Vocoder", true));
    
    createAndAddParameter(std::make_unique<AudioParameterBool>(ParameterID("settings_do_rotate", 1), "PV: Window rotate", true));
    
    createAndAddParameter(std::make_unique<AudioParameterBool>(ParameterID("settings_use_threaded_fft", 1), "PV: Multithread large fft", true));
    
    createAndAddParameter(std::make_unique<AudioParameterBool>(ParameterID("settings_use_hann_linear", 1), "PV: Use linear hann", false));
    
    createAndAddParameter(std::make_unique<AudioParameterInt>(ParameterID(SETTINGS_OVERLAP_NAME, 1), "Number of overlaps", 1, 8, SpectralAudioPlugin::FFT_OVERLAPS));
    
    addParameterListener(SETTINGS_OVERLAP_NAME, this);
    //getNumberOfOverlaps().addListener(this);
}

void PlaygroundParameters::parameterChanged(const juce::String &parameterID, float newValue) {
    if(parameterID != SETTINGS_OVERLAP_NAME) { return; }
    // this previously did something...now doesn't. who cares
}
