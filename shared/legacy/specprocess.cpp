#include "specprocess.h"

#pragma once
#include "utilities.h"

template <class T>
Polar<T> Polar<T>::operator*(T mult){
    Polar<T> result;
    result.m_mag = m_mag * mult;
    result.m_phase = m_phase * mult;
    return result;
}

template<class T>
void car2Pol(std::vector<Cpx> cpxIn, std::vector< Polar<T> >& polOut, int size){
    for(int i=0; i<size; ++i){
        polOut[i].m_mag = abs(cpxIn[i]);
        polOut[i].m_phase = arg(cpxIn[i]);
    }
}

template<class T>
void pol2Car(const std::vector<Polar<T> >& polarIn, std::vector<Cpx>& cpxOut, int size){
    for(int i=0; i<size; ++i){
        float mag = polarIn[i].m_mag;
        float phase = polarIn[i].m_phase;
        cpxOut[i] = std::polar(mag, phase);
        //and perform the reflection here.
        cpxOut[(size*2)-1-i] = 0.f;
    }
}

template <class T>
Polar<T> interp_Polar(const Polar<T>* v, int lim, float i, bool wrap = false){
    if (wrap){
      while( (int)i > lim) i -= float(lim);
    }

    float t;
    float r = modff(i, &t);
    int n = int(t);
    Polar<T> polOut;
    if ( n< lim-1){
        polOut.m_mag = (1.0f-r)*v[n].m_mag + (r)*v[n+1].m_mag;
        polOut.m_phase = (1.0f-r)*v[n].m_phase + (r)*v[n+1].m_phase;
        return polOut;
    }
    polOut.m_mag = 0.f;
    polOut.m_phase = 0.f;
    return polOut;
}

void emptyPolar(std::vector<Polar<float> >& inOutPol){
    Polar<float> empty(0.f, 0.f);
    std::fill(inOutPol.begin(), inOutPol.end(), empty);
}

//----STFT----

STFT::STFT(int size, int hopSize, int offset, int sRate) :
    m_sRate(sRate), m_fftSize(size), m_halfSize(size / 2), m_invSize(1.f/float(size)),
    m_hopsize(hopSize), m_offset(offset), m_hann(size), m_input(size),
    m_cpxInput(size), m_fftOut(size), m_polarOut(m_halfSize), m_polarIn(m_halfSize),
    m_ifftin(size), m_cpxOutput(size), m_output(size),
    fft(new kissfft<float>(m_fftSize, false)), ifft(new kissfft<float> (m_fftSize, true))
{
    fillHann(m_hann, m_fftSize);
}


int STFT::setFFTSize(int newSize){

    m_fftSize = newSize;
    m_halfSize = newSize/2;
    m_invSize = 1.f/(float)newSize;

    Cpx emptyCpx(0, 0);
    m_input.resize(newSize, 0.f);
    m_cpxInput.resize(newSize, emptyCpx);
    m_output.resize(newSize, 0.f);
    m_fftOut.resize(newSize, emptyCpx);
    m_hann.resize(newSize, 0.f);

    fillHann(m_hann, newSize);

    Polar<float> emptyPolar(0.f, 0.f);
    m_polarOut.resize(newSize, emptyPolar);
    m_polarIn.resize(newSize, emptyPolar);
    m_ifftin.resize(newSize, emptyCpx);
    m_cpxOutput.resize(newSize, emptyCpx);
    if(newFFT(newSize) == 1) return 1;

    return 0;
}


void STFT::fillHann(std::vector<float>& table, int size){
    float w;
    float invert_Cos;
    for(int n = 0; n < size; ++n){
        //hann window -- inverted cosine
        w = (float)TWOPI * ( float(n)/float(size-1) );
        invert_Cos = 1.f - cos(w);
        //normalise to 0 to 1
        table[n] = 0.5f * invert_Cos;
    }
}

int STFT::newFFT(int newSize){
	if(fft>0) delete fft;
    if(ifft>0) delete ifft;

    fft = new kissfft<float>(newSize, false);
    ifft = new kissfft<float>(newSize, true);

    if(fft ||  ifft == 0) return 1;
    return 0;
}

void STFT::fill_in_passOut(const float* audioInput, float* processOutput, int blockSize){

	 // this will write the audio input to the internal buffer of the process.
    float* pWriteToProcess = (&m_input[0]) + m_offset;
    float* pWriteToOutput = (&m_output[0]) + m_offset;

    for(int i=0; i<blockSize; ++i){
        *pWriteToProcess = audioInput[i];
        processOutput[i] += *pWriteToOutput;
        ++pWriteToProcess;
        ++pWriteToOutput;
    }
}


void STFT::doHann(std::vector<float>& inOut){
  for(int i=0; i<m_fftSize; ++i){
    inOut[i] *= m_hann[i];
  }
}

void STFT::float2Cpx(const std::vector<float>& inFloat, std::vector<Cpx>& outCpx){
  for(int i=0; i<m_fftSize; ++i){
	outCpx[i].real(inFloat[i]);    
	outCpx[i].imag(0.f);
  }
}

void STFT::cpx2Float(const std::vector<Cpx>& inCpx, std::vector<float>& outFloat){
  for(int i=0; i<m_fftSize; ++i){
    outFloat[i] = inCpx[i].real();
  }
}

void STFT::normalise(std::vector<Cpx>& cmpxInOut){
	for(int i=0; i<m_halfSize; ++i){	  		
		float normalisedRealValue = cmpxInOut[i].real() * m_invSize;
		cmpxInOut[i].real(normalisedRealValue);

		float normalisedImaginaryValue = cmpxInOut[i].imag() * m_invSize;
		cmpxInOut[i].imag(normalisedImaginaryValue);
	}
}

//
//void STFT::zeroPadding(std::vector<float> &inFloat, int user_FFTSize){
    // do not like resizing of the vector within this function!!!!
    //inFloat.resize(m_fftSize, 0.f);
//}
//

// STFT process and PVoc process are defined further on for ease of comparison

//----PVoc----

PVoc::PVoc(int size, int hops, int offset, int sRate) : STFT(size, hops, offset, sRate)
{
	// only need to perform this upon the first initialisation
    initPrevPhase();
}

// initialised here
bool PVoc::s_isInitialised(false);
std::vector<float> PVoc::s_prevPhase(0);
void PVoc::initPrevPhase(){
    if(! s_isInitialised){
		// if not initialised then clear and set the size
        s_prevPhase.resize(m_halfSize, 0.f);
        s_isInitialised = true;
    }
}

void PVoc::doHannRotate(std::vector<float>& inOut){
    std::vector<float> temp(m_fftSize);

	/* We are using the bridge of the temp array to store the rotated input
	otherwise the input array will be writing to its own future points.	*/
    for(int i=0; i<m_fftSize; ++i){
        int rotatedIndex = (m_offset+i) % m_fftSize;
        temp[rotatedIndex] = inOut[i]*m_hann[i];
    }
	// output
    for(int i=0; i<m_fftSize; ++i){
        inOut[i] = temp[i];
    }
}

void PVoc::doHannUnrotate(const std::vector<Cpx>& inCpx, std::vector<float>& outFloat){
    for(int i=0; i<m_fftSize; ++i){		
       int rotatedIndex = utilities::wrap(m_offset+i, m_fftSize); //if below 0 then wrap around.
        outFloat[i] = inCpx[rotatedIndex].real() * m_hann[i];
    }
}

void PVoc::phaseDiff2Hertz(std::vector<Polar<float> >& inOut){

    /*  For each bin the difference in phase will be related to the strongest frequency content of that bin.
    If there is no difference then the true frequency will be the center frequency of the bin.  */
    float fac = (float)m_sRate / ((float)m_hopsize * (float)TWOPI ); // radians to hertz factor.
    float scl= (float)TWOPI * ( (float)m_hopsize / float(m_fftSize) ); // Size in radians of the hopsize.

    for(int n = 0; n < m_halfSize; ++n){
        float phi = inOut[n].m_phase;
        float delta = phi - s_prevPhase[n];
        s_prevPhase[n] = phi;
        utilities::wrap_twopi(delta);
        //n * scale will be the correct value in radians for the freq. adding the delta difference
        m_polarOut[n].m_phase = (delta + float(n)*scl)*fac;
    }
}

void PVoc::hertzDiff2Phase(std::vector<Polar<float> >& inOut){
    // Find the difference between the new value in hertz (post spec process) from the center bin.
    // Convert this to the phase value.

    float fac = (float)m_hopsize * ( (float)TWOPI / (float)m_sRate ); //used for scaling the hertz to radians
    float scl= (float)m_sRate / float(m_fftSize); //used to determine the center frequency for each bin.

    for(int i = 0; i < m_halfSize; ++i){
        float delta = (inOut[i].m_phase - float(i) * scl) * fac;
        // difference between the pre spec process and the post spec process phases
        float phi = s_prevPhase[i] + delta;
        s_prevPhase[i] = phi;
    }
}

void STFT::process(const float* input, float* output, int blockSize){

    fill_in_passOut(input, output, blockSize);
    m_offset += blockSize;

    // when the input buffer reaches the fft block size we can fft
    if( m_offset >= m_fftSize ){


        doHann(m_input);
        // here is a good location for a zero padding function
       // zeroPadding(m_input, int(m_fftSize*0.8));
       //m_input.resize(m_fftSize, 0.f);
        float2Cpx(m_input, m_cpxInput);

        fft->transform(&m_cpxInput[0], &m_fftOut[0]);

        normalise(m_fftOut);
        car2Pol(m_fftOut, m_polarOut, m_halfSize);

        spectral_process(m_polarOut, m_polarIn, m_halfSize);

        pol2Car(m_polarIn, m_ifftin, m_halfSize);
        m_ifftin[0] = 0.f;

        ifft->transform(&m_ifftin[0], &m_cpxOutput[0]);

        cpx2Float(m_cpxOutput, m_output);
        doHann(m_output);
        m_offset = 0; //reset count
    }
}

void PVoc::process(const float* input, float* output, int blockSize){

    fill_in_passOut(input, output, blockSize);
    m_offset += blockSize;

    if( m_offset >= m_fftSize ){

        doHannRotate(m_input);
        float2Cpx(m_input, m_cpxInput);

        fft->transform(&m_cpxInput[0], &m_fftOut[0]);

        normalise(m_fftOut);
        car2Pol(m_fftOut, m_polarOut, m_halfSize);
        phaseDiff2Hertz(m_polarOut);

        spectral_process(m_polarOut, m_polarIn, m_halfSize);

        hertzDiff2Phase(m_polarIn);
        pol2Car(m_polarIn, m_ifftin, m_halfSize);
        m_ifftin[0] = 0.f;

        ifft->transform(&m_ifftin[0], &m_cpxOutput[0]);

        doHannUnrotate(m_cpxOutput, m_output);
        m_offset = 0;
    }
}

int PVoc::setFFTSize(int newSize){    
    int status = STFT::setFFTSize(newSize);
    s_prevPhase.resize(newSize/2, 0);
    return status;
}

void frequencyShifter::spectral_process(const std::vector< Polar<float> > &in, std::vector<Polar<float> > &out, int)const {	
	Polar<float> empty(0.f, 0.f);
	std::fill(out.begin(), out.end(), empty);

	int fftSize = getFFTSize();
	if (fftSize == 0) {
		return;
	}
	
    int maxBin = getRate();	

    int binWidth = maxBin / fftSize;
	int shiftFrequency = m_shift;

	int shift = shiftFrequency / binWidth;	
	
	if (shift == 0) {		
		for (int i = 0; i < in.size(); ++i) {
			out[i] = in[i];
		}
	}
	else if (shift > 0) {
		for (int i = 0; i < in.size() - shift; ++i) {
			out[i + shift] = in[i];
		}
	}
	else if (shift < 0) {
		shift = abs(shift);
		for (int i = shift; i < in.size(); ++i) {
			out[i - shift] = in[i];
		}
	}
}

//----------------
void spectralGate::spectral_process(const std::vector< Polar<float> > &in, std::vector<Polar<float> > &out, int bins)const{
	// a spectral gate
	float mag;
	//float gate = params[0];
	float gate = m_cutOff;
	float balance = m_balance;
	// mags below the low value will be counted as a weak magnitude. I am testing against a much lower value than gate
	// to enchane to difference between strong and weak frequency components
	float gate_low = gate * 0.6f;
	gate = pow(gate, 10);
	float balanceStrong = powf(balance, 3.f);
	float balanceWeak = 1.f-balance;
    balanceWeak = pow(balanceWeak, 4.f);

	out[0]=in[0];
	for(int n = 1; n < bins; ++n){
            mag = in[n].m_mag;
            if (mag >= gate) out[n].m_mag = mag * balanceStrong;
            else if(mag<gate_low) out[n].m_mag = mag*balanceWeak;
            else out[n].m_mag = 0.f;
            out[n].m_phase = in[n].m_phase;
	}



};
//-------
 void sinusoidalShapedFilter::spectral_process(const std::vector< Polar<float> > &in, std::vector<Polar<float> > &out, int bins)const{

    
	const float freq = m_freq + 1.f;    
	const float theta = m_phase * (float)PI;    
	const float width = (m_width * 8.f) + 1.f;

	float w;    
    float sinusoid;

    for(int i=0; i<bins; ++i){
        w = float(i)/float(bins-1);
        w *= (float)TWOPI * freq;
        sinusoid = std::max(0.f, cos(w-theta));
        out[i].m_mag = in[i].m_mag * pow(sinusoid, width);
        out[i].m_phase = in[i].m_phase;
    }
}

//-------
void frequencyMagnet::spectral_process(const std::vector< Polar<float> > &in, std::vector<Polar<float> > &out, int bins)const {
    /*Frequency magnet dynamically merges all bins to a single bin. It is intended to explore spectral width. The x-axis controls the `width'.
    The y-axis controls the target frequency. When the width is zero all frequencies are shifted to a single bin.
    When the width is one the full un-shifted spectrum is presented. The interest comes from the range in between the extremes.
    When the width is at 0.5 the bins below the target frequency are shifted up in varying amounts.
    The bins above the target frequency are shifted lower by varying amounts. So we can hear a focus upon a single frequency emerge. */

    // we will use the width bias input to limit and curve the width paramter

    const float widthBias = m_widthBias*3.f; //this will be the exponent for the width
    const float widthLimit = 1.f-pow(m_widthBias, 0.125f); // this is the lower limit for the width (less width, narrower band ... higher volume!)
	
	int target_bin = (int)((m_freq / (float)this->getRate()) * (float)m_fftSize);
	float width = m_width;
    width = utilities::clip(width, widthLimit, 1.f);
    width = pow(width, widthBias);	

    float line;    	
    float index_below;
    float index_above;

    std::vector<Polar<float> > temp(bins);
    emptyPolar(temp);
    emptyPolar(out);

    //This loop will work on the frequencies below the target freq.
    for(int i=0; i<target_bin; ++i){
        line = float(i)/target_bin;
        // The less width the more convex shape the line will become. Biasing towards one value. If width is zero index will always be one.        
		
		// scale
        index_below = (pow(line, width*8.f / 8.f) * (float)target_bin);

        temp[(int)index_below].m_mag += in[i].m_mag;
        out[i].m_phase = in[i].m_phase;
        out[(int)index_below].m_mag = utilities::interp_lin(temp[(int)index_below].m_mag, temp[(int)index_below+1].m_mag, (float)index_below);
    }

	if(target_bin < 1) target_bin = 1;

	const int range = bins - target_bin;
    for(int i=target_bin; i<bins; ++i){
		/*out[i].m_phase = in[i].m_phase;
		out[i].m_mag = in[i].m_mag;
		continue;*/
        
        // norm_index will run from 0 - Range.
        int norm_index = i-target_bin;
        line = float(norm_index)/float(range);   
        
		// scale
        index_above = (pow(line, ((1.f - width)*7.f) + 1.f)  * (float)range) ;
        // offset by the target frequency.
        index_above += (float)target_bin;

		index_above = utilities::clip(index_above, 1.f, out.size() - 1.f);

        temp[index_above].m_mag += in[i].m_mag;
        out[i].m_phase = in[i].m_phase;
        out[(int)index_above].m_mag = utilities::interp_lin(temp[(int)index_above].m_mag, temp[(int)index_above-1].m_mag, index_above);
    }
}

void binScrambler::spectral_process(const std::vector< Polar<float> > &in, std::vector<Polar<float> > &out, int bins)const {

	const int phase = m_phase;
	const int maxPhase = m_maxPhase;

    for(int i=0; i<bins; ++i){		        
		out[i].m_mag = utilities::interp_lin<float>(in[m_AInd->at(i)].m_mag, in[m_BInd->at(i)].m_mag, (float)phase/(float)maxPhase);
        out[i].m_phase = utilities::interp_lin<float>(in[m_AInd->at(i)].m_phase, in[m_BInd->at(i)].m_phase, (float)phase/(float)maxPhase);
    }
}

void emptyProcess::spectral_process(const std::vector< Polar<float> >& in, std::vector<Polar<float> >&out, int bins)const{
    for(int i=0; i<bins; ++i){
        out[i] = in[i];
    }
}
