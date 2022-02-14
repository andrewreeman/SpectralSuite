#include "PlaygroundParameters.h"
#include "../shared/SpectralAudioPlugin.h"

const String PlaygroundParameters::SETTINGS_OVERLAP_NAME = "settings_overlap_count";

PlaygroundParameters::PlaygroundParameters(SpectralAudioPlugin * processor) :
    PluginParameters(processor), m_processor(processor)
{
    createAndAddParameter(std::make_unique<AudioParameterFloat>(
        "mix",
        "Generic mix parameter",
        0.0,
        1.0,
        1.0
    ));
    
    createAndAddParameter(std::make_unique<AudioParameterBool>("settings_use_pvoc", "Use Phase Vocoder", true));
    
    createAndAddParameter(std::make_unique<AudioParameterBool>("settings_do_rotate", "PV: Window rotate", true));
    
    createAndAddParameter(std::make_unique<AudioParameterBool>("settings_use_threaded_fft", "PV: Multithread large fft", true));
    
    createAndAddParameter(std::make_unique<AudioParameterBool>("settings_use_hann_linear", "PV: Use linear hann", false));
    
    createAndAddParameter(std::make_unique<AudioParameterInt>(SETTINGS_OVERLAP_NAME, "Number of overlaps", 1, 8, SpectralAudioPlugin::FFT_OVERLAPS));
    
    addParameterListener(SETTINGS_OVERLAP_NAME, this);
    //getNumberOfOverlaps().addListener(this);
}

void PlaygroundParameters::parameterChanged(const juce::String &parameterID, float newValue) {
    if(parameterID != SETTINGS_OVERLAP_NAME) { return; }

    auto newOverlapCount = (int)getNumberOfOverlaps().getValue();
    if(newOverlapCount != m_processor->getOverlapCount()) {
        m_processor->switchOverlapCountAsync();
    }
}
