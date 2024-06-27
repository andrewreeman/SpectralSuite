#include "FrequencyMagnetParameters.h"
#include "PitchInfoParser.h"

FrequencyMagnetParameters::FrequencyMagnetParameters(AudioProcessor * processor) : PluginParameters(processor) {
    
    const String freqHertzLabel = "Hz";
    const String shiftHertzLabel = " Hz";
    const float minRangeDefault = 20.0f;
    const float maxRangeDefault = 2000.0f;
    const float increment = 1.0f;

    createAndAddParameter(std::make_unique<AudioParameterFloat>(
        ParameterID("magMinRange", 1),
        "Frequency magnet minimum range",
        NormalisableRange<float>(0, 20000, increment), minRangeDefault, "",
        AudioProcessorParameter::Category::genericParameter,
        [shiftHertzLabel](float v, int) { return String(v, 2) + shiftHertzLabel; },
        [shiftHertzLabel](const String& text) { return text.dropLastCharacters(shiftHertzLabel.length()).getFloatValue(); }
    ));
    
    createAndAddParameter(std::make_unique<AudioParameterFloat>(
        ParameterID("magMaxRange", 1),
        "Frequency magnet maximum range",
        NormalisableRange<float>(0, 20000, increment), maxRangeDefault, "",
        AudioProcessorParameter::Category::genericParameter,
        [shiftHertzLabel](float v, int) { return String(v, 2) + shiftHertzLabel; },
        [shiftHertzLabel](const String& text) { return text.dropLastCharacters(shiftHertzLabel.length()).getFloatValue(); }
    ));
    

    
    createAndAddParameter(std::make_unique<AudioParameterFloat>(
                                                                ParameterID("freq", 1),
                                                                "Frequency",
                                                                NormalisableRange<float>(20.0f, 2000.0f), 800.0f, "",
                                                                AudioProcessorParameter::Category::genericParameter,
                                                                [freqHertzLabel](float v, int) { return PitchInfoParser::generateLabel(v);},
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

void FrequencyMagnetParameters::updateValue(FreqSlider* frequencyMagSlider, double valueToUpdateTo)
{
    AudioParameterFloat* freqParam = (AudioParameterFloat*)getParameter("freq");
    float currentValue = (float)valueToUpdateTo;

    const AudioParameterFloat* lowestValueParam = (AudioParameterFloat*)getParameter("magMinRange");
    const AudioParameterFloat* highestValueParam = (AudioParameterFloat*)getParameter("magMaxRange");

    float lowestValue = lowestValueParam->get();
    float highestValue = highestValueParam->get();
    
    if (lowestValue >= highestValue)
    {
        std::swap(lowestValue, highestValue);
    }

    if (currentValue < lowestValue) {
        currentValue = lowestValue;
    }

    if (currentValue > highestValue) {
        currentValue = highestValue;
    }

    freqParam->range.start = lowestValue;
    freqParam->range.end = highestValue;
    frequencyMagSlider->setRange(freqParam->range.start, freqParam->range.end, freqParam->range.interval);
    freqParam->setValueNotifyingHost(freqParam->convertTo0to1(currentValue));
}
