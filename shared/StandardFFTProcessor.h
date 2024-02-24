#pragma once

#ifndef __include_libs__
#define __include_libs__
#include <cstring>
#include <complex>
#include <cmath>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include "PhaseBuffer.h"
#include "FftThread.h"
#include "Polar.h"
#include "Types.h"
#include "FftWindowType.h"
#endif

#ifndef __constants__
#define __constants__
#define ROOT2OVER2 0.70710678
#define HALFPI 1.570796325
#define TWOPI 6.28318531
#define PI 3.14159265
#endif

#define __specprocesses__

class StandardFFTProcessor{
    // TODO: really? can none of these be private?
protected:
    int m_sRate;
    int m_fftSize;
    int m_halfSize; // FFT/2
    float m_invSize; // 1/FFT
    int m_hopsize;
    int m_offset; // m_offset is used for the initial offset of the STFT instances. This allows overlapping
    FftWindowType m_windowType;
    std::vector<FftDecimal> m_window;
    std::vector<FftDecimal> m_input; // audio input ...
    std::vector<Cpx> m_cpxInput; // ...converted to complex...
    std::vector<Cpx>m_fftOut; //... perform fft  ...
    PolarVector m_polarIn; // ... converted to Polar for the spectral process ...
    PolarVector m_polarOut; // ... output of the spectral process ...
    std::vector<Cpx> m_ifftin; // ... convert to  complex and perform ifft ...
    std::vector<Cpx> m_cpxOutput; // ... output of ifft ...
    std::vector<FftDecimal> m_output; // ... convert to float and pass to output
    std::unique_ptr<kissfft<FftDecimal>> fft;
    std::unique_ptr<kissfft<FftDecimal>> ifft;
    
    // deletes the kissfft objects and creates new ones of the new size
    int newFFT(int newSize);
    
    // fill the inputs and pass to the outputs
    virtual void fill_in_passOut(const FftDecimal* audioInput, FftDecimal* processOutput, int blockSize);
    
    void fillWindow(std::vector<FftDecimal>& table, int size);
    void fillHann(std::vector<FftDecimal>& table, int size);
    void fillBlackman(std::vector<FftDecimal>& table, int size);
    void fillHamming(std::vector<FftDecimal>& table, int size);
    void fillBlackmanHarris(std::vector<FftDecimal>& table, int size);
    void float2Cpx(const std::vector<FftDecimal>& inFloat, std::vector<Cpx>& outCpx);
    void cpx2Float(const std::vector<Cpx>& inCpx, std::vector<FftDecimal>& outFloat);
    void normalise(std::vector<Cpx>& cpxInOut);
    void applyWindow(std::vector<FftDecimal>& inOut);

public:

    /* Constructor for STFT. Every STFT can take an offset in samples as an argument
    The offsets should be (FFTSIZE/(n/i)). Where n is the total number of overlaps and i is the current overlap. */
    StandardFFTProcessor(int size, int hops, int offset, int sRate);

    virtual ~StandardFFTProcessor(){
        fft = nullptr;
        ifft = nullptr;
    }

    void setSampleRate(int newRate){ m_sRate = newRate;}
    int getSampleRate() const { return m_sRate; }
    
    void setWindowType(FftWindowType newType);
    FftWindowType getWindowType() const { return m_windowType; }

    virtual bool setFFTSize(int newSize);
    int getFFTSize() const { return m_fftSize; }

    int getHopSize() const { return m_hopsize; }
    void setHopSize(int hopSize){ m_hopsize = hopSize;}
    void setOffset(int offset){ m_offset = offset; }

    virtual void process(const FftDecimal* input, FftDecimal* output, int blockSize);
    // spectral_process is a pure function that needs to be defined by class derivatives
    virtual void spectral_process(const PolarVector &in, PolarVector &out, int bins) = 0;
};
