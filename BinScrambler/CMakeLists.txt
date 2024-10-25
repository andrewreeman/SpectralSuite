# juce_set_aax_sdk_path(...)

add_plugin(BinScrambler "Scramble the frequency components of a signal" SPBS
    "Bin Scrambler")
    
target_sources(BinScrambler
    PRIVATE
    Source/BinScramblerAudioPlugin.cpp
    Source/BinScramblerAudioPlugin.h
    Source/BinScramblerFFTProcessor.cpp
    Source/BinScramblerFFTProcessor.h
    Source/BinScramblerInteractor.cpp
    Source/BinScramblerInteractor.h
    Source/BinScramblerParameters.cpp
    Source/BinScramblerParameters.h
    Source/RateSlider.cpp
    Source/RateSlider.h
    Source/ScatterSlider.cpp
    Source/ScatterSlider.h
    Source/ScrambleSlider.cpp
    Source/ScrambleSlider.h
    Source/SliderContainer.cpp
    Source/SliderContainer.h
    Source/SliderContainer.h
    ${SHARED_SOURCES}
)
