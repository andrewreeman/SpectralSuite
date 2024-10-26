# juce_set_aax_sdk_path(...)

add_plugin(Ssf "Filters the frequency components by applying a sinusoudal shape over the spectrum." SPSS "Sinusoidal Shaped Filter")

target_sources(Ssf
    PRIVATE
    Source/FreqSlider.h
    Source/FreqSlider.cpp
    Source/PhaseSlider.h
    Source/PhaseSlider.cpp
    Source/SliderContainer.h
    Source/SliderContainer.cpp
    Source/SSF_FFTProcessor.h
    Source/SSF_FFTProcessor.cpp
    Source/SSFAudioPlugin.h
    Source/SSFAudioPlugin.cpp
    Source/SSFInteractor.h
    Source/SSFInteractor.cpp
    Source/SSFParameters.h
    Source/SSFParameters.cpp
    Source/WidthSlider.h
    Source/WidthSlider.cpp
    ${SHARED_SOURCES}
)
