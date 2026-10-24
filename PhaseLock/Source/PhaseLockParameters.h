#pragma once

#include "../../shared/PluginParameters.h"
#include "JuceHeader.h"

class PhaseLockParameters : public PluginParameters {
public:
    using PButtonAttachment = AudioProcessorValueTreeState::ButtonAttachment*;
    using PSliderAttachment = AudioProcessorValueTreeState::SliderAttachment*;

	PhaseLockParameters(AudioProcessor* processor);

    // All getters below read lock-free parameter storage, so they are safe to
    // call on the audio thread. They must not use Value/ValueTree reads here:
    // those allocate, mutate global listener lists, and read unlocked shared
    // state while the message thread writes it.
	bool isPhaseLocked() const { return getBoolParam (KEY_PHASE_LOCK); }
    float getPhaseMix() const { return getFloatParam (KEY_PHASE_MIX, 1.f); }

    bool isFreqLocked() const { return getBoolParam (KEY_MAG_LOCK); }
    float getMagMix() const { return getFloatParam (KEY_MAG_MIX, 1.f); }

    float getMagTrack() const { return getFloatParam (KEY_MAG_TRACK, 0.f); }
    float getRandomPhase() const { return getFloatParam (KEY_RAND_PHASE, 0.f); }

    bool shouldMorphMagAndPhase() const { return getBoolParam (KEY_MORPH_MAG_AND_PHASE); }
    int getMorphDuration() const { return getIntParam (KEY_MORPH_DURATION, 2); }
    
    bool willContinuePlayingWhenSilentInput() const { return isFreqLocked() || shouldMorphMagAndPhase(); }
    
    PButtonAttachment createPhaseLockAttachment(ToggleButton& button) { return createButtonAttachment(KEY_PHASE_LOCK, button); }
    PSliderAttachment createPhaseMixAttachment(Slider& slider) { return createSliderAttachmentByParameterId(KEY_PHASE_MIX, slider); }
    
    PButtonAttachment createMagLockAttachment(ToggleButton& button) { return createButtonAttachment(KEY_MAG_LOCK, button); }
    PSliderAttachment createMagMixAttachment(Slider& slider) { return createSliderAttachmentByParameterId(KEY_MAG_MIX, slider); }
    
    PSliderAttachment createMagTrackAttachment(Slider& slider) { return createSliderAttachmentByParameterId(KEY_MAG_TRACK, slider); }
    PSliderAttachment createRandPhaseAttachment(Slider& slider) { return createSliderAttachmentByParameterId(KEY_RAND_PHASE, slider); }
        
    PButtonAttachment createMorphMagAndPhaseAttachment(ToggleButton& button){ return createButtonAttachment(KEY_MORPH_MAG_AND_PHASE, button); }
    PSliderAttachment createMorphDurationAttachment(Slider& slider) { return createSliderAttachmentByParameterId(KEY_MORPH_DURATION, slider); }
    
private:
    bool getBoolParam (const ParameterID& key) const
    {
        auto* param = getParameter (key.getParamID());
        jassert (param != nullptr);
        return param != nullptr && static_cast<AudioParameterBool*> (param)->get();
    }

    float getFloatParam (const ParameterID& key, float fallback) const
    {
        auto* param = getParameter (key.getParamID());
        jassert (param != nullptr);
        return param != nullptr ? static_cast<AudioParameterFloat*> (param)->get() : fallback;
    }

    int getIntParam (const ParameterID& key, int fallback) const
    {
        auto* param = getParameter (key.getParamID());
        jassert (param != nullptr);
        return param != nullptr ? static_cast<AudioParameterInt*> (param)->get() : fallback;
    }

    using ParameterKey = ParameterID;

    static ParameterKey KEY_PHASE_LOCK;
    static ParameterKey KEY_MAG_LOCK;
    
    static ParameterKey KEY_PHASE_MIX;
    static ParameterKey KEY_MAG_MIX;
    
    static ParameterKey KEY_MAG_TRACK;
    static ParameterKey KEY_RAND_PHASE;
    
    static ParameterKey KEY_MORPH_MAG_AND_PHASE;
    static ParameterKey KEY_MORPH_DURATION;
    
};
