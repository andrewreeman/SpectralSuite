#TODO: very strange having '../shared' prefix. Is there a way to avoid this?
#TODO: add as a static library instead, but had trouble adding juce header

set(
    SHARED_RESOURCES
    "../shared/resources/baseline-arrow_back-24px.svg" 
    "../shared/resources/baseline-info-24px.svg"
    "../shared/resources/baseline-settings-20px.svg"
)

set(
    SHARED_SOURCES

    "../shared/AboutPage.h"
    "../shared/AboutPage.cpp"
    "../shared/AudioValueTreeOnLoadListener.h"
    "../shared/FftInteractor.h"
    "../shared/FftOverlapsChoiceAdapter.h"
    "../shared/FftSizeChoiceAdapter.h"
    "../shared/FftStyleChoiceAdapter.h"
    "../shared/FftSwitcher.h"
    "../shared/FftThread.h"
    "../shared/FftWindowChoiceAdapter.h"
    "../shared/FftWindowType.h"
    "../shared/NullLogger.h"
    "../shared/ParameterContainerComponent.h"
    "../shared/ParameterContainerComponentFactory.h"
    "../shared/PhaseBuffer.h"
    "../shared/PhaseVocoder.h"
    "../shared/PluginParameters.h"
    "../shared/Polar.h"
    "../shared/ResourceRepository.h"
    "../shared/SettingsPage.h"
    "../shared/SpectralAudioPlugin.h"
    "../shared/SpectralAudioPluginUi.h"
    "../shared/SpectralAudioProcessorInteractor.h"
    "../shared/SpectralPluginUiInterface.h"
    "../shared/StandardFFTProcessor.h"
    "../shared/Types.h"
    "../shared/VersionCheck.h"
    "../shared/VersionInfo.h"
    "../shared/delaybuffer.h"
    "../shared/editorshowupdateinterface.h"
    "../shared/utilities.h"
    "../shared/wavetable.h"
    "../shared/FftInteractor.cpp"
    "../shared/FftOverlapsChoiceAdapter.cpp"
    "../shared/FftSizeChoiceAdapter.cpp"
    "../shared/FftStyleChoiceAdapter.cpp"
    "../shared/FftThread.cpp"
    "../shared/FftWindowChoiceAdapter.cpp"
    "../shared/ParameterContainerComponent.cpp"
    "../shared/PhaseVocoder.cpp"
    "../shared/Polar.cpp"
    "../shared/SettingsPage.cpp"
    "../shared/SpectralAudioPlugin.cpp"
    "../shared/SpectralAudioPluginUi.cpp"
    "../shared/SpectralAudioProcessorInteractor.cpp"
    "../shared/StandardFFTProcessor.cpp"
    "../shared/Types.cpp"
    "../shared/VersionCheck.cpp"
    "../shared/editorshowupdateinterface.cpp"
    "../shared/utilities.cpp"
    "../shared/wavetable.cpp"
    "../shared/components/ControlPointComponent.h"
    "../shared/components/LicensesComponent.h"
    "../shared/components/Spline.h"
    "../shared/components/SplineHelper.h"
    "../shared/components/ControlPointComponent.cpp"
    "../shared/components/LicensesComponent.cpp"
    "../shared/components/Spline.cpp"
    "../shared/components/SplineHelper.cpp"
    "../shared/logging/NullLogger.h"
    "../shared/logging/LoggerFactory.h"
    "../shared/logging/LoggerFactory.cpp"
)

