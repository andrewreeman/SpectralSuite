#include "FrequencyMagnetParameters.h"
#include "PitchInfoParser.h"

FrequencyMagnetParameters::FrequencyMagnetParameters(AudioProcessor * processor) : PluginParameters(processor) {
    
    const String freqHertzLabel = "Hz";
    createAndAddParameter(std::make_unique<AudioParameterFloat>(
                                                                ParameterID("freq", 1),
                                                                "Frequency",
                                                                NormalisableRange<float>(20.0f, 2000.0f), 800.0f, "",
                                                                AudioProcessorParameter::Category::genericParameter,
                                                                //		[freqHertzLabel](float v, int) { return String(v, 2) + " " + freqHertzLabel; },
                                                                [freqHertzLabel](float v, int) { return PitchInfoParser::generateLabel(v);},
                                                                //[freqHertzLabel](const String& text) { return text.dropLastCharacters(freqHertzLabel.length()).getFloatValue(); }
                                                                [freqHertzLabel](const String& text) { return PitchInfoParser::parseFrequency(text.toStdString()); }
                                                                ));
    
    createAndAddParameter(std::make_unique<AudioParameterFloat>(
                                                                ParameterID("bias", 1),
                                                                "Width bias",
                                                                NormalisableRange<float>(0.0f, 1.0f), 0.01f, "",
                                                                AudioProcessorParameter::Category::genericParameter
                                                                ));
    
    createAndAddParameter(std::make_unique<AudioParameterFloat>(
                                                                ParameterID("width", 1),
                                                                "Width",
                                                                NormalisableRange<float>(0.0f, 1.0f), 0.5f, "",
                                                                AudioProcessorParameter::Category::genericParameter
                                                                ));
    
    createAndAddParameter(std::make_unique<AudioParameterBool>(
                                                               ParameterID("useLegacyHighFrequencyShift", 1),
                                                               "Use legacy mode",
                                                               false
                                                               ));
}

void FrequencyMagnetParameters::setFrequency(float frequency) {
    getParameterAsValue("freq").setValue(frequency);
}

void FrequencyMagnetParameters::setWidth(float width) {
    getParameterAsValue("width").setValue(width);
}
