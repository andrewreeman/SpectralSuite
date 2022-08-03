#include "SSFParameters.h"

SSFParameters::SSFParameters(AudioProcessor * processor) : PluginParameters(processor) {
    const String freqHertzLabel = "Hz";
    createAndAddParameter(
                          std::make_unique<AudioParameterFloat>(
                                                                ParameterID("freq", 1),
                                                                "Frequency",
                                                                NormalisableRange<float>(0.0f, 10.0f),
                                                                7.0f,
                                                                "",
                                                                AudioProcessorParameter::Category::genericParameter,
                                                                [freqHertzLabel](float v, int) { return String(v, 2) + freqHertzLabel; },
                                                                [freqHertzLabel](const String& text) { return text.dropLastCharacters(freqHertzLabel.length()).getFloatValue(); }
                                                                )
                          );
    
    
    createAndAddParameter(
                          std::make_unique<AudioParameterFloat>(
                                                                ParameterID("width", 1),
                                                                "Width",
                                                                NormalisableRange<float>(0.0f, 1.0f),
                                                                0.9f,
                                                                "",
                                                                AudioProcessorParameter::Category::genericParameter
                                                                )
                          );
    
    //    const String piLabel = " pi";
    createAndAddParameter(
                          std::make_unique<AudioParameterFloat>(
                                                                ParameterID("phase", 1),
                                                                "Phase",
                                                                NormalisableRange<float>(0.0f, 1.0f),
                                                                0.5f,
                                                                "",
                                                                AudioProcessorParameter::Category::genericParameter//,
                                                                //        [piLabel](float v, int) { return String(v, 2) + piLabel; },
                                                                //        [piLabel](const String& text) { return text.dropLastCharacters(piLabel.length()).getFloatValue(); }
                                                                ));
    
}



