#include "StandardFFTProcessor.h"

//#pragma once
#include "utilities.h"

StandardFFTProcessor::StandardFFTProcessor(int size, int hopSize, int offset, int sRate) :
    m_sRate(sRate), m_fftSize(size), m_halfSize(size / 2), m_invSize(1.f/float(size)),
    m_hopsize(hopSize), m_offset(offset), m_windowType(FftWindowType::HANN), m_window((size_t)size), m_input((size_t)size),
    m_cpxInput((size_t)size), m_fftOut((size_t)size), m_polarIn((size_t)m_halfSize),m_polarOut((size_t)m_halfSize),
    m_ifftin((size_t)size), m_cpxOutput((size_t)size), m_output((size_t)size),
    fft(std::make_unique<kissfft<FftDecimal>>(m_fftSize, false)), ifft(std::make_unique<kissfft<FftDecimal>>(m_fftSize, true))
{
    fillWindow(m_window, m_fftSize);
}


void StandardFFTProcessor::process(const FftDecimal* input, FftDecimal* output, int blockSize){

    fill_in_passOut(input, output, blockSize);
    m_offset += blockSize;

    // when the input buffer reaches the fft block size we can fft
    if( m_offset >= m_fftSize ){
        applyWindow(m_input);
        // here is a good location for a zero padding function
       // zeroPadding(m_input, int(m_fftSize*0.8));
       //m_input.resize(m_fftSize, 0.f);
        float2Cpx(m_input, m_cpxInput);

        fft->transform(&m_cpxInput[0], &m_fftOut[0]);

        normalise(m_fftOut);
        utilities::car2Pol(m_fftOut, m_polarOut, m_halfSize);

        spectral_process(m_polarOut, m_polarIn, m_halfSize);

        utilities::pol2Car(m_polarIn, m_ifftin, m_halfSize);
        m_ifftin[0] = 0.f;

        ifft->transform(&m_ifftin[0], &m_cpxOutput[0]);
                
        cpx2Float(m_cpxOutput, m_output);
        applyWindow(m_output);
        m_offset = 0; //reset count
    }
}

void StandardFFTProcessor::setWindowType(FftWindowType newType) {
    m_windowType = newType;
    m_window.clear();
    fillWindow(m_window, m_fftSize);
}

bool StandardFFTProcessor::setFFTSize(int newSize){
    m_fftSize = newSize;
    m_halfSize = newSize/2;
    m_invSize = 1.f/(float)newSize;

    Cpx emptyCpx(0, 0);
    m_input.resize((size_t)newSize, 0.f);
    m_cpxInput.resize((size_t)newSize, emptyCpx);
    m_output.resize((size_t)newSize, 0.f);
    m_fftOut.resize((size_t)newSize, emptyCpx);
    m_window.resize((size_t)newSize, 0.f);

    fillWindow(m_window, newSize);

    Polar<FftDecimal> emptyPolar(0.f, 0.f);
    m_polarOut.resize((size_t )newSize, emptyPolar);
    m_polarIn.resize((size_t)newSize, emptyPolar);
    m_ifftin.resize((size_t)newSize, emptyCpx);
    m_cpxOutput.resize((size_t)newSize, emptyCpx);
    if(newFFT(newSize) == 1) return false;

    return true;
}


void StandardFFTProcessor::fillWindow(std::vector<FftDecimal>& table, int size){
    switch(m_windowType) {
        case FftWindowType::HANN: {
            fillHann(table, size);
            break;
        }
        case BLACKMAN: {
            fillBlackman(table, size);
            break;
        }
        case HAMMING: {
            fillHamming(table, size);
            break;
        }
        case BLACKMAN_HARRIS: {
            fillBlackmanHarris(table, size);
            break;
        }
    }
}

void StandardFFTProcessor::fillHann(std::vector<FftDecimal>& table, int size){
    float w;
    float invert_Cos;
    float max = (float)size - 1.f;
    for(int n = 0; n < size; ++n){
        //hann window -- inverted cosine
        w = (float)TWOPI * ( float(n)/max );
        invert_Cos = 1.f - cos(w);
        //normalise to 0 to 1
        table[(size_t)n] = 0.5f * invert_Cos;
    }
}

void StandardFFTProcessor::fillBlackman(std::vector<FftDecimal>& table, int size) {
    float max = (float)size - 1.f;
    for(int n=0; n < size; ++n) {
        float w1 = (float)TWOPI * (float(n)/max);
        float w2 = (float)TWOPI*2.f * (float(n)/max);
        float c1 = 0.5f * cosf(w1);
        float c2 = 0.08f * cosf(w2);
        table[(size_t)n] = 0.42f - (c1 + c2);
    }
}

void StandardFFTProcessor::fillHamming(std::vector<FftDecimal>& table, int size) {
    float max = (float)size + 1.f;
    for(int n=0; n < size; ++n) {
        float w = (float)TWOPI * (float(n) / max);
        table[(size_t)n] = 0.53836f - 0.46164f * cosf(w);
    }
}

void StandardFFTProcessor::fillBlackmanHarris(std::vector<FftDecimal>& table, int size) {
    float max = (float)size - 1.f;
    for(int n=0; n<size; ++n) {
        float increment = (float(n)/max);
        float w1 = (float)TWOPI * increment;
        float w2 = (float)TWOPI*2.f * increment;
        float w3 = (float)TWOPI*6.f * increment;
        
        float c1 = 0.48829f * cosf(w1);
        float c2 = 0.14128f * cosf(w2);
        float c3 = 0.01168f * cosf(w3);
        
        table[(size_t)n] = 0.35875f - c1 + c2 - c3;
    }
}

int StandardFFTProcessor::newFFT(int newSize){    
    fft = std::make_unique<kissfft<FftDecimal>>(newSize, false);

// TODO: for the time stretching plugin we only will delete the inverse fft array when the stretch amount changes
//    if(ifft != nullptr) {
//       delete ifft;
//       ifft = nullptr;
//    }
    //ifft = new kissfft<float>(524288, true);

    ifft = std::make_unique<kissfft<FftDecimal>>(newSize, true);
    
    if(fft == nullptr || ifft == nullptr) return 1;
    return 0;
}

void StandardFFTProcessor::fill_in_passOut(const FftDecimal* audioInput, FftDecimal* processOutput, int blockSize){

	 // this will write the audio input to the internal buffer of the process.
    FftDecimal* pWriteToProcess = (&m_input[0]) + m_offset;
    FftDecimal* pWriteToOutput = (&m_output[0]) + m_offset;

    for(int i=0; i<blockSize; ++i){
        *pWriteToProcess = audioInput[i];
        processOutput[i] += *pWriteToOutput;
        ++pWriteToProcess;
        ++pWriteToOutput;
    }
}


void StandardFFTProcessor::applyWindow(std::vector<FftDecimal>& inOut){
  for(int i=0; i<m_fftSize; ++i){
    inOut[(size_t)i] *= m_window[(size_t)i];
  }
}

void StandardFFTProcessor::float2Cpx(const std::vector<FftDecimal>& inFloat, std::vector<Cpx>& outCpx){
  for(int i=0; i<m_fftSize; ++i){
	outCpx[(size_t)i].real(inFloat[(size_t)i]);    
	outCpx[(size_t)i].imag(0.f);
  }
}

void StandardFFTProcessor::cpx2Float(const std::vector<Cpx>& inCpx, std::vector<FftDecimal>& outFloat){
  for(int i=0; i<m_fftSize; ++i){
    outFloat[(size_t)i] = inCpx[(size_t)i].real();
  }
}

void StandardFFTProcessor::normalise(std::vector<Cpx>& cmpxInOut){
    
    float scale = m_invSize * 2.f;
	for(int i=0; i<m_halfSize; ++i){	  		
        float normalisedRealValue = cmpxInOut[(size_t)i].real() * scale;
		cmpxInOut[(size_t)i].real(normalisedRealValue);

        float normalisedImaginaryValue = cmpxInOut[(size_t)i].imag() * scale;
		cmpxInOut[(size_t)i].imag(normalisedImaginaryValue);
	}
    
}
