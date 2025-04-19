# juce_set_aax_sdk_path(...)

add_plugin(Morph "Manipulate the shape of the frequency content" SPMO "Morph")

target_sources(Morph
    PRIVATE
    Source/ControlPoints.h
    Source/MorphAudioPlugin.h
    Source/MorphAudioPlugin.cpp
    Source/MorphFFTProcessor.h
    Source/MorphFFTProcessor.cpp
    Source/MorphInteractor.h
    Source/MorphInteractor.cpp
    Source/MorphPluginParameters.h
    Source/MorphPluginParameters.cpp
    Source/MorphSlider.h
    Source/MorphSlider.cpp
    ${SHARED_SOURCES}
)
