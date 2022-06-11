#pragma once

#include "../../shared/StandardFFTProcessor.h"
#include "../../shared/PhaseVocoder.h"
#include "../../shared/PhaseBuffer.h"

class FrequencyShiftFFTProcessor : public PhaseVocoder {
public:
    FrequencyShiftFFTProcessor(int size, int hops, int offset, int sRate, std::shared_ptr<PhaseBuffer> phaseBuffer);
    
    virtual void spectral_process(const PolarVector &in, PolarVector &out, int bins) override;
    virtual bool setFFTSize(int newSize) override;
    
    void setShift(float shift) {
        if(m_shift != (int)shift) {
            m_shift = (int)shift;
            recalculateInternalParameters();
        }
    }
private:
    void recalculateInternalParameters();
    
    int m_shift;
    int m_binShift;
    int m_shiftStartIndex;
    int m_shiftEndIndex;
};
