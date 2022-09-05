#pragma once

#include "../../shared/PhaseVocoder.h"
#include "../../shared/PhaseBuffer.h"

class SpectralGateFFTProcessor: public PhaseVocoder {
    
public:
    SpectralGateFFTProcessor(int size, int hops, int offset, int sRate, std::shared_ptr<PhaseBuffer> phaseBuffer);
    
    virtual void spectral_process(const PolarVector &in, PolarVector &out, int bins) override;
    
    void setCutOff(float cutOff);
    void setBalance(float balance);
    void setTilt(float tilt);
    void setTiltEnabled(bool enabled);
    
private:
    float m_cutOff;
    float m_balance;
    float m_tilt;
    bool m_tiltEnabled;
    
    float m_gate_high;
    
    // Magnitudes below this low value will be counted as a weak magnitude. Testing against a much lower value than the gate cut off will enhance the difference between strong and weak frequency components
    // TODO: make this and the gate high parameters instead of cut off.
    float m_gate_low;
    
    float m_balance_strong_scale;
    float m_balance_weak_scale;
    
    void recalculateInternalParameters();
};
