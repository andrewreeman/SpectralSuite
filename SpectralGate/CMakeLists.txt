# juce_set_aax_sdk_path(...)

add_plugin(SpecGate "Only allow frequency components above or below a particular cut-off amplitude." SPSG "Spectral Gate")

target_sources(SpecGate
    PRIVATE
    Source/BalanceSlider.h
    Source/BalanceSlider.cpp
    Source/CutOffSlider.h
    Source/CutOffSlider.cpp
    Source/SliderContainer.h
    Source/SliderContainer.cpp
    Source/SpectralGateAudioPlugin.h
    Source/SpectralGateAudioPlugin.cpp
    Source/SpectralGateFFTProcessor.h
    Source/SpectralGateFFTProcessor.cpp
    Source/SpectralGateInteractor.h
    Source/SpectralGateInteractor.cpp
    Source/SpectralGateParameters.h
    Source/SpectralGateParameters.cpp
    Source/TiltSlider.h
    Source/TiltSlider.cpp
    ${SHARED_SOURCES}
)
