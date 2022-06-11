#pragma once

#include "../../shared/PhaseVocoder.h"

class MorphFFTProcessor : public PhaseVocoder {
public:
    MorphFFTProcessor(int size, int hops, int offset, int sRate, std::shared_ptr<PhaseBuffer> phaseBuffer, Array<int>** pMorphPointsPointer);    
    
    virtual void spectral_process(const PolarVector &in, PolarVector &out, int bins) override;            
private:
    Array<int>** m_pMorphPointsPointer;
};
