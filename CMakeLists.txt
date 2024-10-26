cmake_minimum_required(VERSION 3.22)
project(SpectralSuite VERSION 2.1.12)
SET(VERSION_CODE 18)

set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs)
include(cmake/cpm.cmake)
CPMAddPackage(
    NAME JUCE
    GIT_TAG 8.0.2
    VERSION 8.0.2
    GITHUB_REPOSITORY juce-framework/JUCE
    SOURCE_DIR ${LIB_DIR}/juce
)

include(shared/shared.cmake)
set(CMAKE_OSX_ARCHITECTURES arm64 x86_64)


function (add_plugin name description code displayName)
juce_add_plugin("${name}"
    COMPANY_NAME "Stepwise"                          
    COMPANY_COPYRIGHT "2021"
    COMPANY_WEBSITE "www.andrewreeman.com"
    COMPANY_EMAIL "andrew.reeman@gmail.com"
    DESCRIPTION "${description}"
    IS_SYNTH FALSE                       
    NEEDS_MIDI_INPUT TRUE 
    NEEDS_MIDI_OUTPUT TRUE 
    IS_MIDI_EFFECT FALSE                 
    COPY_PLUGIN_AFTER_BUILD FALSE        
    PLUGIN_MANUFACTURER_CODE STPW               
    PLUGIN_CODE "${code}"                            # A unique four-character plugin id with exactly one upper-case character
    # GarageBand 10.3 requires the first letter to be upper-case, and the remaining letters to be lower-case
    FORMATS AU VST3 # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
    VST3_CATEGORIES Fx
    AU_MAIN_TYPE kAudioUnitType_MusicEffect
    PRODUCT_NAME "${displayName}")

target_compile_features("${name}" INTERFACE cxx_std_17)
juce_generate_juce_header("${name}")

target_compile_definitions("${name}"
    PUBLIC
    JUCE_WEB_BROWSER=0 
    JUCE_USE_CURL=0 
    JUCE_VST3_CAN_REPLACE_VST2=0
    VersionCode=${VERSION_CODE}
)

juce_add_binary_data("${name}Data" SOURCES ${SHARED_RESOURCES})

target_link_libraries(${name}
    PRIVATE
    "${name}Data"
    juce::juce_audio_utils
    PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags)

endfunction()

add_subdirectory(BinScrambler)
add_subdirectory(FrequencyMagnet)
add_subdirectory(FrequencyShift)
add_subdirectory(Morph)
add_subdirectory(PhaseLock)
add_subdirectory(SinusoidalShapedFilter)

# TODO: all plugins
# TODO: update README

# TODO: add shared as a static library instead, but had trouble adding juce
# header. Confirm can debug ok etc

# TODO: very strange shared resources having '../shared' prefix. Is there a way to avoid this?
