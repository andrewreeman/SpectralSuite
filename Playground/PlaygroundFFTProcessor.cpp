#include "PlaygroundFFTProcessor.h"
#include "../shared/utilities.h"

//#define STRETCH_USING_LARGE_OUTPUT_FFT

PlaygroundFFTProcessor::PlaygroundFFTProcessor(int size, int hops, int offset, int sRate, std::shared_ptr<PhaseBuffer> _phaseBuffer, std::shared_ptr<FftThread> fftThread) : PhaseVocoder(size, hops, offset, sRate, _phaseBuffer), m_initialOffset(offset), m_doWindowRotate(true), m_isWaitingForFft(false), m_useThreadForLargeFft(false),
    m_windowType(WindowType::HANN)
{
    m_prevOutput.resize(size, 0.f);
    phaseBuffer = _phaseBuffer;
    m_fftThread = fftThread;
    m_fftThread->addWorker(this);
    
    #ifdef STRETCH_USING_LARGE_OUTPUT_FFT
    ifft = new kissfft<FftDecimal> (524288, true);
    m_stretchedOutput.resize(524288 /* 2^19, just above 10 seconds at 44.1khz */, 0.f);
    Cpx emptyCpx(0, 0);
    m_expandedInput.resize(524288, emptyCpx);
    m_stretchedOutputCpx.resize(524288, emptyCpx);
    
    // move past end to trigger fft on first run
    m_stretchedOutputReadIndex = (int)m_stretchedOutput.size() + 1;
#else
    ifft = new kissfft<FftDecimal> (size, true);
#endif
}


void PlaygroundFFTProcessor::process(const FftDecimal* input, FftDecimal* output, int blockSize){
    if(!phaseBuffer->isAvailable()) {
        PhaseVocoder::process(input, output, blockSize);
        return;
    }
        
#ifdef STRETCH_USING_LARGE_OUTPUT_FFT
    if(m_stretchedOutputReadIndex < m_stretchedOutput.size()) {
        for(int i=0; i<blockSize; i++) {
            output[i] = m_stretchedOutput[(i + m_stretchedOutputReadIndex)  % m_stretchedOutput.size()];
        }
        
        m_stretchedOutputReadIndex += blockSize;
        return;
    }
#endif
    
    if(m_useThreadForLargeFft && isLargeFft() && m_isWaitingForFft) {
        // this will write the audio input to the internal buffer of the process.
        float* pWriteToOutput = (&m_prevOutput[0]) + m_offset;

        for(int i=0; i<blockSize; ++i){
            output[i] += *pWriteToOutput;
            ++pWriteToOutput;
        }
        m_offset += blockSize;
        
        if( m_offset >= m_fftSize){
            m_offset = 0;
        }
    }
    else {
        fill_in_passOut(input, output, blockSize);
        m_offset += blockSize;
    }

    if( m_offset >= m_fftSize){
        // single fft thread
        // set busy
        // do async if large fft size. do sync if small
        if(m_useThreadForLargeFft && isLargeFft()) {
            readyForFft();
        }
        else {
            performFftWork();
        }
        
        // set offset sync?
        m_offset = 0;
    }
}

bool PlaygroundFFTProcessor::setFFTSize(int newSize){
    bool status = PhaseVocoder::setFFTSize(newSize);
    m_prevOutput.resize(newSize, 0.f);
    phaseBuffer->requestResize(newSize/2);
    return status;
}

void PlaygroundFFTProcessor::yield() {
    if(m_fftThread->isThreadRunning()) {
        m_fftThread->yield();
    }
}

void PlaygroundFFTProcessor::fill_in_passOut(const FftDecimal* audioInput, FftDecimal* processOutput, int blockSize) {
    int offset = m_offset;
    if(offset >= m_fftSize) {
        offset = offset % m_fftSize;
    }
    
	 // this will write the audio input to the internal buffer of the process.
    FftDecimal* pWriteToProcess = (&m_input[0]) + offset;
    FftDecimal* pWriteToOutput = (&m_output[0]) + offset;

    for(int i=0; i<blockSize; ++i){
        *pWriteToProcess = audioInput[i];
        processOutput[i] += *pWriteToOutput;
        ++pWriteToProcess;
        ++pWriteToOutput;
    }
}

void PlaygroundFFTProcessor::doHannRotate(std::vector<FftDecimal>& inOut) {
    if(m_doWindowRotate) {
        PhaseVocoder::doHannRotate(inOut);
    }
}

void PlaygroundFFTProcessor::doHannUnrotate(const std::vector<Cpx>& inCpx, std::vector<FftDecimal>& outFloat) {
    if(m_doWindowRotate) {
        PhaseVocoder::doHannUnrotate(inCpx, outFloat);
    }
    else {
        cpx2Float(inCpx, outFloat);
        applyWindow(outFloat);
    }
}


void PlaygroundFFTProcessor::performFftWork() {
    doHannRotate(m_input);
    float2Cpx(m_input, m_cpxInput);
    yield();
    
    fft->transform(&m_cpxInput[0], &m_fftOut[0]);
    yield();
    
    normalise(m_fftOut);
    utilities::car2Pol(m_fftOut, m_polarOut, m_halfSize);
    phaseDiff2Hertz(m_polarOut);
    yield();
    
    spectral_process(m_polarOut, m_polarIn, m_halfSize);
    
    hertzDiff2Phase(m_polarIn);
    utilities::pol2Car(m_polarIn, m_ifftin, m_halfSize);
    yield();
    m_ifftin[0] = 0.f;

#ifdef STRETCH_USING_LARGE_OUTPUT_FFT
    int expansionDiff = (int)m_expandedInput.size() / (int)m_ifftin.size();
//    for(int i=0; i < m_expandedInput.size(); i++) {
//        float inIndexF = (float)i / (float)expansionDiff;
//
//        if((int)inIndexF >= m_ifftin.size() - 2) { break; }
//
//        float aReal = m_ifftin[inIndexF].real();
//        float bReal = m_ifftin[inIndexF + 1].real();
//        float interpReal = utilities::interp_lin<float>(aReal, bReal, inIndexF);
//
//        m_expandedInput[i].real(interpReal);
//
//        float aImag = m_ifftin[inIndexF].imag();
//        float bImag = m_ifftin[inIndexF + 1].imag();
//        float interpImag = utilities::interp_lin<float>(aImag, bImag, inIndexF);
//
//        m_expandedInput[i].imag(interpImag);
//    }
    
    for(int i=0, j=0; i<m_expandedInput.size() && j<m_ifftin.size(); i += expansionDiff, j++) {
        FftDecimal mag = abs(m_ifftin[j]);
//        float phase = arg(m_ifftin[i]);
        FftDecimal phase = (FftDecimal)(rand() % 7);
                
        m_expandedInput[i] = std::polar(mag, phase);
//        m_expandedInput[i].real(m_ifftin[j].real());
        
    }
    yield();
//
    //ifft->transform(&m_ifftin[0], &m_cpxOutput[0]);
    ifft->transform(&m_expandedInput[0], &m_stretchedOutputCpx[0]);
    yield();
    
    for(int i=0; i<m_stretchedOutputCpx.size(); ++i){
        m_stretchedOutput[i] = m_stretchedOutputCpx[i].real();
    }
    yield();
    
    m_stretchedOutputReadIndex = 0;
#else
    ifft->transform(&m_ifftin[0], &m_cpxOutput[0]);
    doHannUnrotate(m_cpxOutput, m_output);
    m_offset = 0; //reset count
#endif
    if(isLargeFft()) {
        m_prevOutput = m_output;
        m_isWaitingForFft = false;
    }
}

void PlaygroundFFTProcessor::readyForFft() {
    m_isWaitingForFft = true;
    m_fftThread->notify();
}
