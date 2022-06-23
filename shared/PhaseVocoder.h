#pragma once

#include "StandardFFTProcessor.h"


//-----------------A phase vocoder derivative of STFT-----------------------------------------//
/*
In this class we perform the phase vocoder during the STFT process. This allows for a much finer frequency resolution
and manipulation
*/
class PhaseVocoder : public StandardFFTProcessor {
protected:
    /* Rotate the input in preparation for finding the difference in phase between the ideal center frequency
    and the most prominant frequency content */
    virtual void doHannRotate(std::vector<FftDecimal>& inOut);
    // unrotate for passing the data to the output
    virtual void doHannUnrotate(const std::vector<Cpx>& inCpx, std::vector<FftDecimal>& outFloat);
    virtual void phaseDiff2Hertz(PolarVector& inOut);
    virtual void hertzDiff2Phase(PolarVector&);
    
public:

    PhaseVocoder(int size, int hops, int offset, int sRate, std::shared_ptr<PhaseBuffer> _phaseBuffer);
    ~PhaseVocoder() {}
    
    virtual void setUsePvoc(bool usePvoc){
        if(this->m_fftSize > 32768) {
            phaseBuffer->setUsePvoc(false);
            return;
        }
        
        phaseBuffer->setUsePvoc(usePvoc);
    }
    
    virtual bool setFFTSize(int newSize) override;
    virtual void process(const FftDecimal* input, FftDecimal* output, int blockSize) override;
            
    // still an abstract class that needs spectral_process to be overwritten
    virtual void spectral_process(const PolarVector &in, PolarVector &out, int bins) override = 0;
    virtual void setOffset(int offset){
        m_initialOffset = offset;
        StandardFFTProcessor::setOffset(offset);
    }
    
private:
    virtual void doFFTWork();
    
    std::shared_ptr<PhaseBuffer> phaseBuffer;
    int m_initialOffset;
};
