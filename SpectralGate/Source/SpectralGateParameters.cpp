#include "SpectralGateParameters.h"

SpectralGateParameters::SpectralGateParameters(AudioProcessor *processor) : PluginParameters(processor) {
    const String dbSuffix = " dB";
    createAndAddParameter(std::make_unique<AudioParameterFloat>(
        ParameterID("cutoff", 1),
        "Cutoff",
        NormalisableRange(0.0f, 1.0f),
        0.6f,
        AudioParameterFloatAttributes()
        .withLabel("")
        .withCategory(AudioProcessorParameter::Category::genericParameter)
        .withStringFromValueFunction(
            [dbSuffix](float value, int) {
                return String(Decibels::gainToDecibels(value), 0) + dbSuffix;
            })
        .withValueFromStringFunction(
            [dbSuffix](const String &text) {
                return Decibels::decibelsToGain(text.dropLastCharacters(dbSuffix.length()).getFloatValue());
            })
    ));

    createAndAddParameter(std::make_unique<AudioParameterFloat>(
        ParameterID("balance", 1),
        "Weak/Strong Balance",
        NormalisableRange(0.0f, 1.0f),
        0.7f,
        AudioParameterFloatAttributes()
        .withLabel("")
        .withCategory(AudioProcessorParameter::Category::genericParameter)
    ));

    createAndAddParameter(std::make_unique<AudioParameterFloat>(
        ParameterID("tilt", 2),
        "Tilt",
        NormalisableRange<float>(0.0f, 1.0f),
        0.5f,
        AudioParameterFloatAttributes()
        .withLabel("")
        .withCategory(AudioProcessorParameter::Category::genericParameter)
    ));

    createAndAddParameter(std::make_unique<AudioParameterBool>(
        ParameterID("enableTilt", 2),
        "Enable tilt",
        false
    ));
}

bool SpectralGateParameters::getTiltEnabled() const {
    const auto tiltEnabledParam = dynamic_cast<AudioParameterBool *>(getParameter("enableTilt"));
    if (tiltEnabledParam != nullptr) {
        return tiltEnabledParam->get();
    } else {
        return false;
    }
}
