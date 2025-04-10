#include "FrequencyShiftPluginParameters.h"

FrequencyShiftPluginParameters::FrequencyShiftPluginParameters(AudioProcessor *processor) : PluginParameters(
    processor) {
    const String shiftHertzLabel = " Hz";
    constexpr float minRangeDefault = -500.0f;
    constexpr float maxRangeDefault = 500.0f;
    constexpr float increment = 1.0f;

    createAndAddParameter(std::make_unique<AudioParameterFloat>(
        ParameterID("shiftMinRange", 1),
        "Frequency shift minimum range",
        NormalisableRange<float>(-20000, -10, increment),
        minRangeDefault,
        AudioParameterFloatAttributes()
        .withLabel("")
        .withCategory(AudioProcessorParameter::Category::genericParameter)
        .withStringFromValueFunction(
            [shiftHertzLabel](const float v, int) { return String(v, 2) + shiftHertzLabel; }
        )
        .withValueFromStringFunction(
            [shiftHertzLabel](const String &text) {
                return text.dropLastCharacters(shiftHertzLabel.length()).getFloatValue();
            }
        )
    ));

    createAndAddParameter(std::make_unique<AudioParameterFloat>(
        ParameterID("shiftMaxRange", 1),
        "Frequency shift maximum range",
        NormalisableRange<float>(10, 20000, increment),
        maxRangeDefault,
        AudioParameterFloatAttributes()
        .withLabel("")
        .withCategory(AudioProcessorParameter::Category::genericParameter)
        .withStringFromValueFunction(
            [shiftHertzLabel](const float v, int) { return String(v, 2) + shiftHertzLabel; }
        )
        .withValueFromStringFunction(
            [shiftHertzLabel](const String &text) {
                return text.dropLastCharacters(shiftHertzLabel.length()).getFloatValue();
            }
        )
    ));

    createAndAddParameter(std::make_unique<AudioParameterFloat>(
        ParameterID("shift", 1),
        "Frequency shift",
        NormalisableRange(minRangeDefault, maxRangeDefault, increment),
        0.0f,
        AudioParameterFloatAttributes()
        .withLabel("")
        .withCategory(AudioProcessorParameter::Category::genericParameter)
        .withStringFromValueFunction([shiftHertzLabel](const float v, int) { return String(v, 2) + shiftHertzLabel; })
        .withValueFromStringFunction([shiftHertzLabel](const String &text) {
            return text.dropLastCharacters(shiftHertzLabel.length()).getFloatValue();
        })
    ));
}

void FrequencyShiftPluginParameters::updateValue(Slider *frequencyShiftSlider, double valueToUpdateTo) {
    AudioParameterFloat *shiftParam = (AudioParameterFloat *) getParameter("shift");
    float currentValue = (float) valueToUpdateTo;

    const AudioParameterFloat *lowestValueParam = (AudioParameterFloat *) getParameter("shiftMinRange");
    const AudioParameterFloat *highestValueParam = (AudioParameterFloat *) getParameter("shiftMaxRange");

    const float lowestValue = lowestValueParam->get();
    const float highestValue = highestValueParam->get();

    if (currentValue < lowestValue) {
        currentValue = lowestValue;
    }

    if (currentValue > highestValue) {
        currentValue = highestValue;
    }

    shiftParam->range.start = lowestValue;
    shiftParam->range.end = highestValue;
    frequencyShiftSlider->setRange(shiftParam->range.start, shiftParam->range.end, shiftParam->range.interval);
    shiftParam->setValueNotifyingHost(shiftParam->convertTo0to1(currentValue));
}
