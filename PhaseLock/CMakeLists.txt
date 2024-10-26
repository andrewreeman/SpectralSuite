# juce_set_aax_sdk_path(...)

add_plugin(PhaseLock "Locks the phase and/or locks the frequency" SPPL "Phase Lock")

target_sources(PhaseLock
    PRIVATE
    Source/PhaseLockAudioPlugin.h
    Source/PhaseLockAudioPlugin.cpp
    Source/PhaseLockFFTProcessor.h
    Source/PhaseLockFFTProcessor.cpp
    Source/PhaseLockInteractor.h
    Source/PhaseLockInteractor.cpp
    Source/PhaseLockParameters.h
    Source/PhaseLockParameters.cpp
    Source/SliderContainer.h
    Source/SliderContainer.cpp
    Source/TransitionState.h
    Source/TransitionState.cpp
    Source/LockState.h
    Source/LockState.cpp
    ${SHARED_SOURCES}
)
