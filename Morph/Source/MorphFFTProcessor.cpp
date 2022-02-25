#include "MorphFFTProcessor.h"
#include "../../shared/Polar.h"

MorphFFTProcessor::MorphFFTProcessor(int size, int hops, int offset, int sRate, Array<int>** pMorphPointsPointer)
    : STFT(size, hops, offset, sRate), m_pMorphPointsPointer(pMorphPointsPointer)
{}

void MorphFFTProcessor::spectral_process(const PolarVector &in, PolarVector &out, int halfSize) {
    for (unsigned int i = 0; i < in.size(); ++i) {
        out[i].m_mag = 0.0;
        out[i].m_phase = 0.0;
    }    
        
    Array<int>* pMorphPoints = *m_pMorphPointsPointer;
    int size  = pMorphPoints->size();
    if(size != halfSize) {
        return;        
    }
    
    for(unsigned int i=0; i<halfSize; ++i) {
        auto mappedIndex = (*pMorphPoints)[i];        
        out[mappedIndex] = in[i];
    }    
}