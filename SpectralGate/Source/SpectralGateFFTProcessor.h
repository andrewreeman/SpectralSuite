#pragma once

#include "../../shared/specprocess.h"

class SpectralGateFFTProcessor: public STFT {
    
public:
    SpectralGateFFTProcessor(int size, int hops, int offset, int sRate);
    
    virtual void spectral_process(const PolarVector &in, PolarVector &out, int bins) override;
    
    void setCutOff(float cutOff);
    void setBalance(float balance);
    
private:
    float m_cutOff;
    float m_balance;
    
    float m_gate_high;
    
    // Magnitudes below this low value will be counted as a weak magnitude. Testing against a much lower value than the gate cut off will enhance the difference between strong and weak frequency components
    // TODO: make this and the gate high parameters instead of cut off.
    float m_gate_low;
    
    float m_balance_strong_scale;
    float m_balance_weak_scale;
    
    void recalculateInternalParameters();
};
