# juce_set_aax_sdk_path(...)

add_plugin(FreqMagnet "Frequency content will gradually shift towards being centered on one frequency" SPFM
    "Frequency Magnet")

target_sources(FreqMagnet
    PRIVATE
    Source/BiasSlider.h
    Source/BiasSlider.cpp
    Source/FreqSlider.h
    Source/FreqSlider.cpp
    Source/FrequencyMagnetAudioPlugin.h
    Source/FrequencyMagnetAudioPlugin.cpp
    Source/FrequencyMagnetFFTProcessor.h
    Source/FrequencyMagnetFFTProcessor.cpp
    Source/FrequencyMagnetInteractor.h
    Source/FrequencyMagnetInteractor.cpp
    Source/FrequencyMagnetParameters.h
    Source/FrequencyMagnetParameters.cpp
    Source/PitchInfoParser.h
    Source/PitchInfoParser.cpp
    Source/SliderContainer.h
    Source/SliderContainer.cpp
    Source/WidthSlider.h
    Source/WidthSlider.cpp
    ${SHARED_SOURCES}
)
