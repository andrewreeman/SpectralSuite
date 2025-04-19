#include "PhaseLockFFTProcessor.h"
#include "../../shared/utilities.h"

static float phaseLockGenerateRandomNumber() {
    float r = static_cast<float>(rand() % 1000);
    float p = (float)((r / 1000.f) * TWOPI);
    return p - (float)PI;
}

PhaseLockFFTProcessor::PhaseLockFFTProcessor(int size, int hops, int offset, int sRate, std::shared_ptr<PhaseBuffer> phaseBuffer) :
		PhaseVocoder(size, hops, offset, sRate, phaseBuffer),
		m_lockedPhases(0),
        m_lockedMag(0),
        m_phaseMix(1.0),
        m_magMix(1.0),
        m_magTrack(0.0),
        m_randomPhaseMix(0.f),
        m_morphDurationSeconds(1),
        m_transitionMagCounter(0.0),
        m_transitionMagEnd(0.0),
        m_transitionPhaseCounter(0.0),
        m_transitionPhaseEnd(0.0),
        m_transitionPhaseState(sRate, size),
        m_transitionMagState(sRate, size)
        {}

void PhaseLockFFTProcessor::spectral_process(const PolarVector& in, PolarVector& out, int bins) {
    doLock(bins, in, out);
    doMorphTransition(out);
}

void PhaseLockFFTProcessor::doLock(int bins, const PolarVector &in, PolarVector &out) {
    if (m_lockPhaseState.shouldTransitionToOn()) {
        m_lockedPhases.clear();
        for (size_t i = 0; i < static_cast<size_t>(bins); ++i) {
            m_lockedPhases.push_back(in[i].m_phase);
        }
        
        m_lockPhaseState.completeTransitionToOn();
    }
    
    if (m_lockMagState.shouldTransitionToOn()) {
        m_lockedMag.clear();
        for (size_t i = 0; i < static_cast<size_t>(bins); ++i) {
            m_lockedMag.push_back(in[i].m_mag);
        }
        
        m_lockMagState.completeTransitionToOn();
    }
    
    float maxMag = 0.f;
    
    for(auto& inBin : in) {
        if(inBin.m_mag > maxMag) {
            maxMag = inBin.m_mag;
        }
    }
    
    float maxLockedMag = 0.1f;
    for(auto& lockedBin : m_lockedMag) {
        if(lockedBin > maxLockedMag) {
            maxLockedMag = lockedBin;
        }
    }
    
    float scale = maxMag / maxLockedMag;
    scale = scale + (-(scale - 1.f) * (1.0f - m_magTrack));
    float magMix = m_magMix;
    float randPhaseMix = m_randomPhaseMix;
    
    if(m_lockPhaseState.isOn() && m_lockMagState.isOn()) {
        for (size_t i = 0; i < static_cast<size_t>(bins); ++i) {
            float inPhase = in[i].m_phase;
            out[i].m_phase = inPhase + ((m_lockedPhases.at(i) - inPhase) * m_phaseMix);
            
            float inMag = in[i].m_mag;
            out[i].m_mag = (inMag + ((m_lockedMag.at(i) - inMag) * magMix)) * scale;
            out[i].m_phase = ( randPhaseMix * phaseLockGenerateRandomNumber()) + ((1.f - randPhaseMix) * out[i].m_phase); }
    }
    else if(m_lockPhaseState.isOn() && m_lockMagState.isOff()) {
        for (size_t i = 0; i < static_cast<size_t>(bins); ++i) {
            out[i] = in[i];
            float inPhase = in[i].m_phase;
            out[i].m_phase = inPhase + ((m_lockedPhases.at(i) - inPhase) * m_phaseMix);
            out[i].m_phase = ( randPhaseMix * phaseLockGenerateRandomNumber()) + ((1.f - randPhaseMix) * out[i].m_phase);
        }
    }
    else if(m_lockPhaseState.isOff() && m_lockMagState.isOn()) {
        for (size_t i = 0; i < static_cast<size_t>(bins); ++i) {
            const float inMag = in[i].m_mag;
            out[i].m_mag = (inMag + ((m_lockedMag.at(i) - inMag) * magMix)) * scale;
            out[i].m_phase = ( randPhaseMix * phaseLockGenerateRandomNumber() ) + ((1.f - randPhaseMix) * in[i].m_phase);
        }
    }
    else {
        for (size_t i = 0; i < static_cast<size_t>(bins); ++i) {
            out[i] = in[i];
            out[i].m_phase = randPhaseMix * phaseLockGenerateRandomNumber() + ((1.f - randPhaseMix) * out[i].m_phase);
        }
    }
}

void PhaseLockFFTProcessor::doMorphTransition(PolarVector &out) {
    if(m_transitionMagState.shouldStartTransition() && m_transitionPhaseState.shouldStartTransition()) {
        m_targetMags.clear();
        m_targetPhases.clear();
        for(auto& input : out) {
            m_targetMags.push_back(input.m_mag);
            m_targetPhases.push_back(input.m_phase);
        }
    }
    else if(m_transitionMagState.shouldStartTransition()) {
        m_targetMags.clear();
        for(auto& input : out) {
            m_targetMags.push_back(input.m_mag);
        }
    }
    else if(m_transitionPhaseState.shouldStartTransition()) {
        m_targetPhases.clear();
        for(auto& input : out) {
            m_targetPhases.push_back(input.m_phase);
        }
    }
    
    if(!m_transitionMagState.isOff() && !m_transitionPhaseState.isOff()) {
        const float phaseInterpolateFactor = m_transitionPhaseState.next(); //std::min<float>(1.f, m_transitionPhaseCounter / m_transitionPhaseEnd);
        const float magInterpolateFactor = m_transitionMagState.next(); //std::min<float>(1.f, m_transitionMagCounter / m_transitionMagEnd);
        
        for(size_t i=0; i < m_targetMags.size(); ++i) {
            out[i].m_mag = utilities::interp_lin_normalised<float>(out[i].m_mag, m_targetMags[i], magInterpolateFactor);
            out[i].m_phase = utilities::interp_lin_normalised<float>(out[i].m_phase, m_targetPhases[i], phaseInterpolateFactor);
        }
    }
}

bool PhaseLockFFTProcessor::setFFTSize(const int newSize) {
    const size_t newVectorSize = static_cast<size_t>(newSize);
    m_lockedPhases.resize(newVectorSize);
    m_lockedMag.resize(newVectorSize);
    
    m_lockPhaseState.reset();
    m_lockMagState.reset();
    
    m_targetPhases.resize(newVectorSize);
    m_targetMags.resize(newVectorSize);
    
    m_transitionPhaseState.setSampleRateAndBlockSize(getSampleRate(), newSize);
    m_transitionMagState.setSampleRateAndBlockSize(getSampleRate(), newSize);
    
    return super::setFFTSize(newSize);
}

void PhaseLockFFTProcessor::lockPhase() {
    m_lockPhaseState.beginTransitionToOn();
}

void PhaseLockFFTProcessor::unlockPhase() {
    m_lockPhaseState.off();
}

void PhaseLockFFTProcessor::lockMag() {
    m_lockMagState.beginTransitionToOn();
}

void PhaseLockFFTProcessor::unlockMag() {
    m_lockMagState.off();
}

void PhaseLockFFTProcessor::startMorphingMag() {
    m_transitionMagState.start();
//    if(m_transitionMagState.isOn()) { return; }
//
//    m_transitionMagEnd = getSampleRate() * m_morphDurationSeconds;
//    m_transitionMagCounter = 0.0;
//    m_transitionMagState.beginTransitionToOn();
}

void PhaseLockFFTProcessor::stopMorphingMag() {
    m_transitionMagState.stop();
//    m_transitionMagState.off();
}

void PhaseLockFFTProcessor::startMorphingPhase() {
    m_transitionPhaseState.start();
//    if(m_transitionPhaseState.isOn()) { return; }
//
//    m_transitionPhaseEnd = getSampleRate() * m_morphDurationSeconds;
//    m_transitionPhaseCounter = 0.0;
//    m_transitionPhaseState.beginTransitionToOn();
}

void PhaseLockFFTProcessor::stopMorphingPhase() {
    m_transitionPhaseState.stop();
//    m_transitionPhaseState.off();
}

