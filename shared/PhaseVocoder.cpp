#include "PhaseVocoder.h"
#include "utilities.h"

PhaseVocoder::PhaseVocoder(int size, int hops, int offset, int sRate, std::shared_ptr<PhaseBuffer> _phaseBuffer) : StandardFFTProcessor(size, hops, offset, sRate), m_initialOffset(offset)
{
    /*
    * StandardFFTProcessor Consturtor has already alloc an iff
    * Why alloc here again?
    * if use raw pointer here this would cause memory leak!
    */
    // ifft = new kissfft<FftDecimal> (size, true);
    phaseBuffer = _phaseBuffer;
}


void PhaseVocoder::doHannRotate(std::vector<FftDecimal>& inOut){
    std::vector<FftDecimal> temp(m_fftSize);

	/* We are using the bridge of the temp array to store the rotated input
	otherwise the input array will be writing to its own future points.	*/
    for(int i=0; i<m_fftSize; ++i){
        int rotatedIndex = (m_initialOffset+i) % m_fftSize;
        temp[rotatedIndex] = inOut[i]*m_window[i];
    }
	// output
    for(int i=0; i<m_fftSize; ++i){
        inOut[i] = temp[i];
    }
}

void PhaseVocoder::doHannUnrotate(const std::vector<Cpx>& inCpx, std::vector<FftDecimal>& outFloat){
    for(int i=0; i<m_fftSize; ++i){
       int rotatedIndex = utilities::wrap(i - m_initialOffset, m_fftSize); //if below 0 then wrap around.
        // previously was...
//        int rotatedIndex (m_offset+i) % m_fftSize;
        outFloat[i] = inCpx[rotatedIndex].real() * m_window[i];        
    }
}

void PhaseVocoder::phaseDiff2Hertz(PolarVector& inOut){

    /*  For each bin the difference in phase will be related to the strongest frequency content of that bin.
    If there is no difference then the true frequency will be the center frequency of the bin.  */
    float fac = (float)m_sRate / ((float)m_hopsize * (float)TWOPI ); // radians to hertz factor.
    float scl= (float)TWOPI * ( (float)m_hopsize / float(m_fftSize) ); // Size in radians of the hopsize.

    for(int n = 0; n < m_halfSize; ++n){
        float phi = inOut[n].m_phase;
        float delta = phi - phaseBuffer->prevPhase[n];
        
        phaseBuffer->prevPhase[n] = phi;
        utilities::wrap_twopi(delta);
        //n * scale will be the correct value in radians for the freq. adding the delta difference
        m_polarOut[n].m_phase = (delta + float(n)*scl)*fac;
    }
}

void PhaseVocoder::hertzDiff2Phase(PolarVector& inOut){
    // Find the difference between the new value in hertz (post spec process) from the center bin.
    // Convert this to the phase value.

    float fac = (float)m_hopsize * ( (float)TWOPI / (float)m_sRate ); //used for scaling the hertz to radians
    float scl= (float)m_sRate / float(m_fftSize); //used to determine the center frequency for each bin.

    for(int i = 0; i < m_halfSize; ++i){
        float delta = (inOut[i].m_phase - float(i) * scl) * fac;
        // difference between the pre spec process and the post spec process phases
        float phi = phaseBuffer->prevPhase[i] + delta;
                
        phaseBuffer->prevPhase[i] = phi;
    }
}


void PhaseVocoder::process(const FftDecimal* input, FftDecimal* output, int blockSize) {
    if(!phaseBuffer->isAvailable()) {
        StandardFFTProcessor::process(input, output, blockSize);
        return;
    }
   
    fill_in_passOut(input, output, blockSize);
    m_offset += blockSize;

    if( m_offset >= m_fftSize){
        // single fft thread
        // set busy
        // do async if large fft size. do sync if small
        doFFTWork();
                
        // set offset sync?
        m_offset = 0;
    }
}


void PhaseVocoder::doFFTWork() {
    doHannRotate(m_input);
    float2Cpx(m_input, m_cpxInput);
    
    fft->transform(&m_cpxInput[0], &m_fftOut[0]);
    
    normalise(m_fftOut);
    utilities::car2Pol(m_fftOut, m_polarOut, m_halfSize);
    phaseDiff2Hertz(m_polarOut);
    
    spectral_process(m_polarOut, m_polarIn, m_halfSize);
    
    hertzDiff2Phase(m_polarIn);
    utilities::pol2Car(m_polarIn, m_ifftin, m_halfSize);
    m_ifftin[0] = 0.f;
            
    ifft->transform(&m_ifftin[0], &m_cpxOutput[0]);
    doHannUnrotate(m_cpxOutput, m_output);
    m_offset = 0; //reset count
}

bool PhaseVocoder::setFFTSize(int newSize){
    if(newSize > 32768) {
        setUsePvoc(false);
    }
    
    bool status = StandardFFTProcessor::setFFTSize(newSize);
    phaseBuffer->requestResize(newSize/2);
    return status;
}

