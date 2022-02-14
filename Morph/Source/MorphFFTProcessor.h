#pragma once

#include "../../shared/specprocess.h"

class MorphFFTProcessor : public STFT {
public:
    MorphFFTProcessor(int size, int hops, int offset, int sRate, Array<int>** pMorphPointsPointer);    
    
    virtual void spectral_process(const PolarVector &in, PolarVector &out, int bins) override;            
private:
    Array<int>** m_pMorphPointsPointer;
};
