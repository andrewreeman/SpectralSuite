# Acceptance tests for the SpectralSuite DSP core.
# These are headless juce::UnitTest classes run through the UnitTestRunner in TestRunner.cpp.

juce_add_console_app(SpectralSuiteTests PRODUCT_NAME "SpectralSuite Tests")

target_include_directories(SpectralSuiteTests
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_sources(SpectralSuiteTests
    PRIVATE
    TestRunner.cpp
    TestSupport.h
    UtilityTests.cpp
    SpectralGateTests.cpp
    FrequencyShiftTests.cpp
    BinScramblerTests.cpp
    FftResizeTests.cpp
    FftResizeProductionTests.cpp
    PhaseLockTests.cpp
    MorphTests.cpp
    SSFTests.cpp
    FrequencyMagnetTests.cpp
    PitchInfoParserTests.cpp

    # Shared DSP core under test
    ${CMAKE_CURRENT_SOURCE_DIR}/../shared/utilities.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../shared/Polar.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../shared/Types.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../shared/wavetable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../shared/StandardFFTProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../shared/PhaseVocoder.cpp
    # Interactor + parameters for the production-faithful resize regression (#97).
    ${CMAKE_CURRENT_SOURCE_DIR}/../shared/SpectralAudioProcessorInteractor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../BinScrambler/Source/BinScramblerParameters.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../BinScrambler/Source/BinScramblerInteractor.cpp

    # Per-plugin spectral processors under test
    ${CMAKE_CURRENT_SOURCE_DIR}/../SpectralGate/Source/SpectralGateFFTProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../FrequencyShift/Source/FrequencyShiftFFTProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../BinScrambler/Source/BinScramblerFFTProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../Morph/Source/MorphFFTProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../SinusoidalShapedFilter/Source/SSF_FFTProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../FrequencyMagnet/Source/FrequencyMagnetFFTProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../FrequencyMagnet/Source/PitchInfoParser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../PhaseLock/Source/PhaseLockFFTProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../PhaseLock/Source/LockState.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../PhaseLock/Source/TransitionState.cpp
)

# Test sources pull in shared UI headers that reference the BinaryData
# namespace, so the test target embeds the same resources as the plugins.
juce_add_binary_data(SpectralSuiteTestsData
    SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/../shared/resources/baseline-arrow_back-24px.svg
    ${CMAKE_CURRENT_SOURCE_DIR}/../shared/resources/baseline-info-24px.svg
    ${CMAKE_CURRENT_SOURCE_DIR}/../shared/resources/baseline-settings-20px.svg
)

# The root sets CMAKE_COMPILE_WARNING_AS_ERROR ON, but JUCE's own bundled
# third-party sources (harfbuzz etc.) do not build warning-free on every
# toolchain. Keep the global policy for plugin targets but opt this test app out.
set_target_properties(SpectralSuiteTests PROPERTIES COMPILE_WARNING_AS_ERROR OFF)

# Match the plugin builds: no web browser or curl, and share the version code.
target_compile_definitions(SpectralSuiteTests
    PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    VersionCode=${VERSION_CODE}
)

juce_generate_juce_header(SpectralSuiteTests)

# Note: we intentionally do not link juce_recommended_warning_flags here.
# Those turn on -Werror -Wsign-conversion, which fights the idiomatic int-indexed
# loops used throughout the tests. Plugin targets still use the full warning set.
target_link_libraries(SpectralSuiteTests
    PRIVATE
    SpectralSuiteTestsData
    juce::juce_audio_utils
    PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
)

add_test(NAME SpectralSuiteTests COMMAND SpectralSuiteTests)
