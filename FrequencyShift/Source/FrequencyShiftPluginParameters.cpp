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

    createAndAddParameter(std::make_unique<AudioParameterFloat>(
        ParameterID("scale", 1),
        "Frequency scale",
        NormalisableRange(0.25f, 3.f, 0.001f, 0.5f, false),
        1.f,
        AudioParameterFloatAttributes()
        .withLabel("")
        .withCategory(AudioProcessorParameter::Category::genericParameter)
    ));
}

void FrequencyShiftPluginParameters::updateValue(Slider *frequencyShiftSlider, const double valueToUpdateTo) const {
    const auto shiftParam = dynamic_cast<AudioParameterFloat *>(getParameter("shift"));
    auto currentValue = static_cast<float>(valueToUpdateTo);

    const AudioParameterFloat *lowestValueParam = dynamic_cast<AudioParameterFloat *>(getParameter("shiftMinRange"));
    const AudioParameterFloat *highestValueParam = dynamic_cast<AudioParameterFloat *>(getParameter("shiftMaxRange"));

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
