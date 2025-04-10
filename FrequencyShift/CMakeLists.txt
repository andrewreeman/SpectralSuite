# juce_set_aax_sdk_path(...)

add_plugin(FreqShift "Shifts frequencies up and down" SPFS
        "Frequency Shifter")

target_sources(FreqShift
        PRIVATE
        Source/FrequencyShiftAudioPlugin.h
        Source/FrequencyShiftAudioPlugin.cpp
        Source/FrequencyShiftFFTProcessor.h
        Source/FrequencyShiftFFTProcessor.cpp
        Source/FrequencyShiftInteractor.h
        Source/FrequencyShiftInteractor.cpp
        Source/FrequencyShiftPluginParameters.h
        Source/FrequencyShiftPluginParameters.cpp
        Source/FrequencySlider.h
        Source/FrequencySlider.cpp
        Source/SliderContainer.h
        Source/SliderContainer.cpp
        ${SHARED_SOURCES}
)
