# Example Audio Plugin CMakeLists.txt

# To get started on a new plugin, copy this entire folder (containing this file and C++ sources) to
# a convenient location, and then start making modifications.

# The first line of any CMake project should be a call to `cmake_minimum_required`, which checks
# that the installed CMake will be able to understand the following CMakeLists, and ensures that
# CMake's behaviour is compatible with the named version. This is a standard CMake command, so more
# information can be found in the CMake docs.

cmake_minimum_required(VERSION 3.22)
# The top-level CMakeLists.txt file for a project must contain a literal, direct call to the
# `project()` command. `project()` sets up some helpful variables that describe source/binary
# directories, and the current project version. This is a standard CMake command.

project(MORPH VERSION 2.1.12)

# If you've installed JUCE somehow (via a package manager, or directly using the CMake install
# target), you'll need to tell this project that it depends on the installed copy of JUCE. If you've
# included JUCE directly in your source tree (perhaps as a submodule), you'll need to tell CMake to
# include that subdirectory as part of the build.

#find_package(JUCE CONFIG REQUIRED)        # If you've installed JUCE to your system
# or
add_subdirectory(JUCE)                    # If you've put JUCE in a subdirectory called JUCE

# If you are building a VST2 or AAX plugin, CMake needs to be told where to find these SDKs on your
# system. This setup should be done before calling `juce_add_plugin`.

# juce_set_vst2_sdk_path(...)
# juce_set_aax_sdk_path(...)

# `juce_add_plugin` adds a static library target with the name passed as the first argument
# (AudioPluginExample here). This target is a normal CMake target, but has a lot of extra properties set
# up by default. As well as this shared code static library, this function adds targets for each of
# the formats specified by the FORMATS arguments. This function accepts many optional arguments.
# Check the readme at `docs/CMake API.md` in the JUCE repo for the full list.

juce_add_plugin(Morph
    # VERSION ...                               # Set this if the plugin version is different to the project version
    # ICON_BIG ...                              # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
    # ICON_SMALL ...
    COMPANY_NAME Stepwise                          # Specify the name of the plugin's author
    COMPANY_COPYRIGHT 2021
    COMPANY_WEBSITE www.andrewreeman.com
    COMPANY_EMAIL andrew.reeman@gmail.com
    DESCRIPTION "Manipulate the shape of the frequency content"
    IS_SYNTH FALSE                       # Is this a synth or an effect?
    NEEDS_MIDI_INPUT TRUE # Does the plugin need midi input?
    NEEDS_MIDI_OUTPUT TRUE # Does the plugin need midi output?
    # IS_MIDI_EFFECT TRUE/FALSE                 # Is this plugin a MIDI effect?
    # EDITOR_WANTS_KEYBOARD_FOCUS TRUE/FALSE    # Does the editor need keyboard focus?
    # COPY_PLUGIN_AFTER_BUILD TRUE/FALSE        # Should the plugin be installed to a default location after building?
    PLUGIN_MANUFACTURER_CODE STPW               # A four-character manufacturer id with at least one upper-case character
    PLUGIN_CODE SPMO                            # A unique four-character plugin id with exactly one upper-case character
    # GarageBand 10.3 requires the first letter to be upper-case, and the remaining letters to be lower-case
    FORMATS AU VST3 # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
    VST3_CATEGORIES Fx
    AU_MAIN_TYPE kAudioUnitType_MusicEffect
    PRODUCT_NAME "Morph")        # The name of the final executable, which can differ from the target name

# `juce_generate_juce_header` will create a JuceHeader.h for a given target, which will be generated
# into your build tree. This should be included with `#include <JuceHeader.h>`. The include path for
# this header will be automatically added to the target. The main function of the JuceHeader is to
# include all your JUCE module headers; if you're happy to include module headers directly, you
# probably don't need to call this.

# TODO: point to single shared JUCE directory for all plugins
# TODO: try on windows
# TODO: try ninja build...and research wtf this is 
# TODO: refactor...use cmakelists in shared and expose library
# TODO: read this file...check every comment by JUCE and remove if actioned
# TODO: read all JUCE api docs for this
# TODO: borrow ideas from here: https://github.com/JanWilczek/audio-plugin-template

target_compile_features(Morph INTERFACE cxx_std_17)
juce_generate_juce_header(Morph)

# `target_sources` adds source files to a target. We pass the target that needs the sources as the
# first argument, then a visibility parameter for the sources which should normally be PRIVATE.
# Finally, we supply a list of source files that will be built into the target. This is a standard
# CMake command.

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

    ../shared/AboutPage.h
    ../shared/AboutPage.cpp

    ../shared/AudioValueTreeOnLoadListener.h
    ../shared/FftInteractor.h
    ../shared/FftOverlapsChoiceAdapter.h
    ../shared/FftSizeChoiceAdapter.h
    ../shared/FftStyleChoiceAdapter.h
    ../shared/FftSwitcher.h
    ../shared/FftThread.h
    ../shared/FftWindowChoiceAdapter.h
    ../shared/FftWindowType.h
    ../shared/NullLogger.h
    ../shared/ParameterContainerComponent.h
    ../shared/ParameterContainerComponentFactory.h
    ../shared/PhaseBuffer.h
    ../shared/PhaseVocoder.h
    ../shared/PluginParameters.h
    ../shared/Polar.h
    ../shared/ResourceRepository.h
    ../shared/SettingsPage.h
    ../shared/SpectralAudioPlugin.h
    ../shared/SpectralAudioPluginUi.h
    ../shared/SpectralAudioProcessorInteractor.h
    ../shared/SpectralPluginUiInterface.h
    ../shared/StandardFFTProcessor.h
    ../shared/Types.h
    ../shared/VersionCheck.h
    ../shared/VersionInfo.h
    ../shared/delaybuffer.h
    ../shared/editorshowupdateinterface.h
    ../shared/utilities.h
    ../shared/wavetable.h

    ../shared/FftInteractor.cpp
    ../shared/FftOverlapsChoiceAdapter.cpp
    ../shared/FftSizeChoiceAdapter.cpp
    ../shared/FftStyleChoiceAdapter.cpp
    ../shared/FftThread.cpp
    ../shared/FftWindowChoiceAdapter.cpp
    ../shared/ParameterContainerComponent.cpp
    ../shared/PhaseVocoder.cpp
    ../shared/Polar.cpp
    ../shared/SettingsPage.cpp
    ../shared/SpectralAudioPlugin.cpp
    ../shared/SpectralAudioPluginUi.cpp
    ../shared/SpectralAudioProcessorInteractor.cpp
    ../shared/StandardFFTProcessor.cpp
    ../shared/Types.cpp
    ../shared/VersionCheck.cpp
    ../shared/editorshowupdateinterface.cpp
    ../shared/utilities.cpp
    ../shared/wavetable.cpp

    ../shared/components/ControlPointComponent.h
    ../shared/components/LicensesComponent.h
    ../shared/components/Spline.h
    ../shared/components/SplineHelper.h

    ../shared/components/ControlPointComponent.cpp
    ../shared/components/LicensesComponent.cpp
    ../shared/components/Spline.cpp
    ../shared/components/SplineHelper.cpp

    ../shared/logging/NullLogger.h
    ../shared/logging/LoggerFactory.h
    ../shared/logging/LoggerFactory.cpp
)

# `target_compile_definitions` adds some preprocessor definitions to our target. In a Projucer
# project, these might be passed in the 'Preprocessor Definitions' field. JUCE modules also make use
# of compile definitions to switch certain features on/off, so if there's a particular feature you
# need that's not on by default, check the module header for the correct flag to set here. These
# definitions will be visible both to your code, and also the JUCE module code, so for new
# definitions, pick unique names that are unlikely to collide! This is a standard CMake command.

target_compile_definitions(Morph
    PUBLIC
    # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
    JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
    JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
    JUCE_VST3_CAN_REPLACE_VST2=0
    VersionCode=18
)

# If your target needs extra binary assets, you can add them here. The first argument is the name of
# a new static library target that will include all the binary resources. There is an optional
# `NAMESPACE` argument that can specify the namespace of the generated binary data class. Finally,
# the SOURCES argument should be followed by a list of source files that should be built into the
# static library. These source files can be of any kind (wav data, images, fonts, icons etc.).
# Conversion to binary-data will happen when your target is built.
juce_add_binary_data(MorphData SOURCES
    "../shared/resources/baseline-arrow_back-24px.svg" 
    "../shared/resources/baseline-info-24px.svg"
    "../shared/resources/baseline-settings-20px.svg"
)
#
# `target_link_librari################################es` links libraries and JUCE modules to other libraries or executables. Here,
# we're linking our executable target to the `juce::juce_audio_utils` module. Inter-module
# dependencies are resolved automatically, so `juce_core`, `juce_events` and so on will also be
# linked automatically. If we'd generated a binary data target above, we would need to link to it
# here too. This is a standard CMake command.

target_link_libraries(Morph
    PRIVATE
    MorphData           # If we'd created a binary data target, we'd link to it here
    juce::juce_audio_utils
    PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags)
