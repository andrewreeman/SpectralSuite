# juce_set_aax_sdk_path(...)

juce_add_plugin(BinScrambler
    COMPANY_NAME Stepwise                          
    COMPANY_COPYRIGHT 2021
    COMPANY_WEBSITE www.andrewreeman.com
    COMPANY_EMAIL andrew.reeman@gmail.com
    DESCRIPTION "Scramble the frequency components of a signal"
    IS_SYNTH FALSE                       
    NEEDS_MIDI_INPUT TRUE 
    NEEDS_MIDI_OUTPUT TRUE 
    IS_MIDI_EFFECT FALSE                 
    COPY_PLUGIN_AFTER_BUILD FALSE        
    PLUGIN_MANUFACTURER_CODE STPW               
    PLUGIN_CODE SPBS                            
    FORMATS AU VST3 
    VST3_CATEGORIES Fx
    AU_MAIN_TYPE kAudioUnitType_MusicEffect
    PRODUCT_NAME "Bin Scrambler")

target_compile_features(BinScrambler INTERFACE cxx_std_17)
juce_generate_juce_header(BinScrambler)

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

target_compile_definitions(BinScrambler
    PUBLIC
    JUCE_WEB_BROWSER=0 
    JUCE_USE_CURL=0 
    JUCE_VST3_CAN_REPLACE_VST2=0
    VersionCode=18
)

juce_add_binary_data(BinScramblerData SOURCES ${SHARED_RESOURCES})

target_link_libraries(BinScrambler
    PRIVATE
    BinScramblerData
    juce::juce_audio_utils
    PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags)
