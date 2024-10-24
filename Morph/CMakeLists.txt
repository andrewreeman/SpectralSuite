# juce_set_aax_sdk_path(...)

juce_add_plugin(Morph
    COMPANY_NAME Stepwise                          
    COMPANY_COPYRIGHT 2021
    COMPANY_WEBSITE www.andrewreeman.com
    COMPANY_EMAIL andrew.reeman@gmail.com
    DESCRIPTION "Manipulate the shape of the frequency content"
    IS_SYNTH FALSE                       
    NEEDS_MIDI_INPUT TRUE 
    NEEDS_MIDI_OUTPUT TRUE 
    IS_MIDI_EFFECT FALSE                 
    COPY_PLUGIN_AFTER_BUILD FALSE        
    PLUGIN_MANUFACTURER_CODE STPW               
    PLUGIN_CODE SPMO                            # A unique four-character plugin id with exactly one upper-case character
    # GarageBand 10.3 requires the first letter to be upper-case, and the remaining letters to be lower-case
    FORMATS AU VST3 # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
    VST3_CATEGORIES Fx
    AU_MAIN_TYPE kAudioUnitType_MusicEffect
    PRODUCT_NAME "Morph")

# TODO: all plugins


target_compile_features(Morph INTERFACE cxx_std_17)
juce_generate_juce_header(Morph)

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

target_compile_definitions(Morph
    PUBLIC
    JUCE_WEB_BROWSER=0 
    JUCE_USE_CURL=0 
    JUCE_VST3_CAN_REPLACE_VST2=0
    VersionCode=18
)

juce_add_binary_data(MorphData SOURCES ${SHARED_RESOURCES})

target_link_libraries(Morph
    PRIVATE
    MorphData           
    juce::juce_audio_utils
    PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags)
