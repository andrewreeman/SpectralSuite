#include "PhaseLockFFTProcessor.h"
#include "../../shared/utilities.h"
#include <algorithm>

static float phaseLockGenerateRandomNumber() {
    float r = static_cast<float>(rand() % 1000);
    float p = (float)((r / 1000.f) * TWOPI);
    return p - (float)PI;
}

PhaseLockFFTProcessor::PhaseLockFFTProcessor(int size, int hops, int offset, int sRate, std::shared_ptr<PhaseBuffer> phaseBuffer) :
		PhaseVocoder(size, hops, offset, sRate, phaseBuffer),
		m_lockedPhases(static_cast<size_t>(size / 2)),
        m_lockedMag(static_cast<size_t>(size / 2)),
        m_phaseMix(1.0),
        m_magMix(1.0),
        m_magTrack(0.0),
        m_randomPhaseMix(0.f),
        m_transitionPhaseState(sRate, size),
        m_transitionMagState(sRate, size)
        {}

void PhaseLockFFTProcessor::spectral_process(const PolarVector& in, PolarVector& out, int bins) {
    doLock(bins, in, out);
    doMorphTransition(out);
}

void PhaseLockFFTProcessor::doLock(int bins, const PolarVector &in, PolarVector &out) {
    const size_t numBins = static_cast<size_t>(bins);

    if (m_lockPhaseState.shouldTransitionToOn()) {
        // resize() only allocates when growing; the storage is pre-sized in
        // the constructor and setFFTSize, so steady-state capture never
        // allocates on the audio thread.
        m_lockedPhases.resize(numBins);
        for (size_t i = 0; i < numBins; ++i) {
            m_lockedPhases[i] = in[i].m_phase;
        }

        m_lockPhaseState.completeTransitionToOn();
    }

    if (m_lockMagState.shouldTransitionToOn()) {
        m_lockedMag.resize(numBins);
        for (size_t i = 0; i < numBins; ++i) {
            m_lockedMag[i] = in[i].m_mag;
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

    // Clamp every locked-spectrum read to what was actually captured. A
    // mismatch can only follow a concurrent resize, and must degrade to
    // passthrough: .at() would throw on the audio thread and terminate.
    const size_t ioBins = std::min({numBins, in.size(), out.size()});
    const size_t safeBins = std::min({ioBins, m_lockedPhases.size(), m_lockedMag.size()});

    if(m_lockPhaseState.isOn() && m_lockMagState.isOn()) {
        for (size_t i = 0; i < safeBins; ++i) {
            float inPhase = in[i].m_phase;
            out[i].m_phase = inPhase + ((m_lockedPhases[i] - inPhase) * m_phaseMix);

            float inMag = in[i].m_mag;
            out[i].m_mag = (inMag + ((m_lockedMag[i] - inMag) * magMix)) * scale;
            out[i].m_phase = ( randPhaseMix * phaseLockGenerateRandomNumber()) + ((1.f - randPhaseMix) * out[i].m_phase); }
    }
    else if(m_lockPhaseState.isOn() && m_lockMagState.isOff()) {
        for (size_t i = 0; i < safeBins; ++i) {
            out[i] = in[i];
            float inPhase = in[i].m_phase;
            out[i].m_phase = inPhase + ((m_lockedPhases[i] - inPhase) * m_phaseMix);
            out[i].m_phase = ( randPhaseMix * phaseLockGenerateRandomNumber()) + ((1.f - randPhaseMix) * out[i].m_phase);
        }
    }
    else if(m_lockPhaseState.isOff() && m_lockMagState.isOn()) {
        for (size_t i = 0; i < safeBins; ++i) {
            const float inMag = in[i].m_mag;
            out[i].m_mag = (inMag + ((m_lockedMag[i] - inMag) * magMix)) * scale;
            out[i].m_phase = ( randPhaseMix * phaseLockGenerateRandomNumber() ) + ((1.f - randPhaseMix) * in[i].m_phase);
        }
    }
    else {
        for (size_t i = 0; i < safeBins; ++i) {
            out[i] = in[i];
            out[i].m_phase = randPhaseMix * phaseLockGenerateRandomNumber() + ((1.f - randPhaseMix) * out[i].m_phase);
        }
    }

    // Bins past the captured spectra pass through unlocked.
    for (size_t i = safeBins; i < ioBins; ++i) {
        out[i] = in[i];
        out[i].m_phase = randPhaseMix * phaseLockGenerateRandomNumber() + ((1.f - randPhaseMix) * out[i].m_phase);
    }
}

void PhaseLockFFTProcessor::doMorphTransition(PolarVector &out) {
    if(m_transitionMagState.shouldStartTransition() || m_transitionPhaseState.shouldStartTransition()) {
        // Both targets are captured together so they can never diverge in
        // size. resize() only allocates when growing; the storage is
        // pre-sized in setFFTSize, so steady-state capture never allocates
        // on the audio thread.
        m_targetMags.resize(out.size());
        m_targetPhases.resize(out.size());
        for(size_t i = 0; i < out.size(); ++i) {
            m_targetMags[i] = out[i].m_mag;
            m_targetPhases[i] = out[i].m_phase;
        }
    }

    if(!m_transitionMagState.isOff() && !m_transitionPhaseState.isOff()
        && !m_targetMags.empty() && m_targetMags.size() == m_targetPhases.size()) {
        const float phaseInterpolateFactor = m_transitionPhaseState.next(); //std::min<float>(1.f, m_transitionPhaseCounter / m_transitionPhaseEnd);
        const float magInterpolateFactor = m_transitionMagState.next(); //std::min<float>(1.f, m_transitionMagCounter / m_transitionMagEnd);

        // Blend only what the output buffer holds; the captured targets may
        // be larger after the output shrank.
        const size_t blendBins = std::min(out.size(), m_targetMags.size());
        for(size_t i=0; i < blendBins; ++i) {
            out[i].m_mag = utilities::interp_lin_normalised<float>(out[i].m_mag, m_targetMags[i], magInterpolateFactor);
            out[i].m_phase = utilities::interp_lin_normalised<float>(out[i].m_phase, m_targetPhases[i], phaseInterpolateFactor);
        }
    }
}

bool PhaseLockFFTProcessor::setFFTSize(const int newSize) {
    const size_t newBins = static_cast<size_t>(newSize / 2);
    m_lockedPhases.resize(newBins);
    m_lockedMag.resize(newBins);
    
    m_lockPhaseState.reset();
    m_lockMagState.reset();

    // Pre-size the morph targets to the output buffer size they will be
    // captured from (StandardFFTProcessor sizes m_polarIn to newSize).
    m_targetPhases.resize(static_cast<size_t>(newSize));
    m_targetMags.resize(static_cast<size_t>(newSize));
    
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

