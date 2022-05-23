#pragma once

#include "../../shared/PhaseVocoder.h"
#include "LockState.h"
#include "TransitionState.h"

class PhaseLockFFTProcessor : public PhaseVocoder {
private:
    typedef PhaseVocoder super;

public:
	PhaseLockFFTProcessor(int size, int hops, int offset, int sRate, std::shared_ptr<PhaseBuffer> phaseBuffer);
    
    void extracted(int bins, const PolarVector &in, PolarVector &out);
    
    void extracted(const PolarVector &in, PolarVector &out);
    
    virtual void spectral_process(const PolarVector& in, PolarVector& out, int bins) override;
    virtual bool setFFTSize(int newSize) override;
	
    void setPhaseMix(float mix) { m_phaseMix = mix; }
    void setMagMix(float mix) { m_magMix = mix; }
    void setMagTrack(float magTrack){ m_magTrack = magTrack; }
    void setRandomPhase(float randPhase){ m_randomPhaseMix = randPhase;  }
    void setMorphDurationSeconds(int seconds){
        /*m_morphDurationSeconds = seconds;*/
        m_transitionPhaseState.setDuration(seconds);
        m_transitionMagState.setDuration(seconds);
    }
 
    void lockPhase();
	void unlockPhase();
    
    void lockMag();
    void unlockMag();
    
    void startMorphingMagAndFreq() {
        startMorphingMag();
        startMorphingPhase();
    }
    
    void stopMorphingMagAndFreq() {
        stopMorphingMag();
        stopMorphingPhase();
    }
    
    void startMorphingMag();
    void stopMorphingMag();
    
    void startMorphingPhase();
    void stopMorphingPhase();

private:
    void doLock(int bins, const PolarVector &in, PolarVector &out);
    void doMorphTransition(PolarVector &out);
    
private:
	std::vector<FftDecimal> m_lockedPhases;
    std::vector<FftDecimal> m_lockedMag;
    
    std::vector<FftDecimal> m_targetPhases;
    std::vector<FftDecimal> m_targetMags;
            
    float m_phaseMix;
    float m_magMix;
    float m_magTrack;
    float m_randomPhaseMix;
    int m_morphDurationSeconds;
    
    float m_transitionMagCounter;
    float m_transitionMagEnd;
    
    float m_transitionPhaseCounter;
    float m_transitionPhaseEnd;
           
    LockState m_lockPhaseState;
    LockState m_lockMagState;
    
//    LockState m_transitionPhaseState;
//    LockState m_transitionMagState;
    TransitionState m_transitionPhaseState;
    TransitionState m_transitionMagState;
};
