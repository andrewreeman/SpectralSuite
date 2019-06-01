/*
Dr D. Moore and Andrew Reeman of the University of Sheffield: Short time fourier transform with hann windowing.
A simple framework to allow inserting of spectral processing within a real-time audio environment.
*/


/*-----
08-05: Modified to include a phase vocoder.
02-06: Structure modified by Andrew Reeman to allow child classes to define the spectral process
08-06: Organised into understandable self-commented routines
-----*/

#pragma once

#ifndef __include_libs__
#define __include_libs__
#include <cstring>
#include <complex>
#include "kissfft.hh"
#include <cmath>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#endif

#ifndef __constants__
#define __constants__
#define ROOT2OVER2 0.70710678
#define HALFPI 1.570796325
#define TWOPI 6.28318531
#define PI 3.14159265
#endif

#define __specprocesses__


// Cpx will be used in this class as a shorthand for the complex type
typedef kissfft<float>::cpx_type Cpx;

//This is a struct that will contain the magnitute and phase within the polar domain
template <class T>
struct Polar{
    T m_mag;
    T m_phase;
    Polar() : m_mag(0), m_phase(0){}

    /* The explicit keyword means that a constructor will be `non-converting'.
   	This means that we can only use the constructor for initializing this class.
	So, We cannot do Polar pol1 = 34 which could assign 34 to mag and 0 to phase.
	We can do Polar pol1(43, 0) or Polar pol1 = Polar(34, 0) */
    explicit Polar(const T& mag, const T& phase){
            m_mag = mag;
            m_phase = phase;
    }
    explicit Polar(const std::complex<T>& cmplx){
        m_mag = abs(cmplx);
        m_phase = arg(cmplx);
    }
    Polar<T> operator* (T mult);
};

//--------------------------------------Standard fourier transform class--------------------------//
class STFT{
   protected:
        int m_sRate;
        int m_fftSize;
        int m_halfSize; // FFT/2
        float m_invSize; // 1/FFT
        int m_hopsize;
		int m_offset; // m_offset is used for the initial offset of the STFT instances. This allows overlapping
        std::vector<float> m_hann;
        std::vector<float> m_input; // audio input ...
        std::vector<Cpx> m_cpxInput; // ...converted to complex...
    	std::vector<Cpx>m_fftOut; //... perform fft  ...
		std::vector<Polar<float> > m_polarIn; // ... converted to Polar for the spectral process ...
		std::vector<Polar<float> > m_polarOut; // ... output of the spectral process ...
		std::vector<Cpx> m_ifftin; // ... convert to  complex and perform ifft ...
        std::vector<Cpx> m_cpxOutput; // ... output of ifft ...
        std::vector<float> m_output; // ... convert to float and pass to output
        kissfft<float>* fft;
        kissfft<float>* ifft;

        int newFFT(int newSize); // deletes the kissfft objects and creates new ones of the new size
        void fill_in_passOut(const float* audioInput, float* processOutput, int blockSize); // fill the inputs and pass to the outputs
        void fillHann(std::vector<float>& table, int size);
        void float2Cpx(const std::vector<float>& inFloat, std::vector<Cpx>& outCpx);
        void cpx2Float(const std::vector<Cpx>& inCpx, std::vector<float>& outFloat);
        void normalise(std::vector<Cpx>& cpxInOut);
        void doHann(std::vector<float>& inOut);
        //
   //     void zeroPadding(std::vector<float>& inFloat, int user_FFTSize);
        

    public:

	    /* Constructor for STFT. Every STFT can take an offset in samples as an argument
  		The offsets should be (FFTSIZE/(n/i)). Where n is the total number of overlaps and i is the current overlap. */
  		STFT(int size, int hops, int offset, int sRate);

        virtual ~STFT(){
            if(fft != nullptr) {
                delete fft;
            }
            
            if(ifft != nullptr) {
                delete ifft;
            }
        }

        void setRate(int newRate){ m_sRate = newRate;}
        int getRate() const { return m_sRate; }

        virtual int setFFTSize(int newSize);
        int getFFTSize() const { return m_fftSize; }

        int getHopSize() const { return m_hopsize; }
        void setHopSize(int hopSize){ m_hopsize = hopSize;}
        void setOffset(int offset){ m_offset = offset; }

        virtual void process(const float* input, float* output, int blockSize);
		// spectral_process is a pure function that needs to be defined by class derivatives
        virtual void spectral_process(const std::vector< Polar<float> > &in, std::vector<Polar<float> > &out, int bins) const = 0;
};

//-----------------A phase vocoder derivative of STFT-----------------------------------------//
/*
In this class we perform the phase vocoder during the STFT process. This allows for a much finer frequency resolution
and manipulation
*/
class PVoc : public STFT{
    private:
		// Previous phases are kept in a static array. This array is zeroed upon initialisation.
        void initPrevPhase();

		/* Rotate the input in preparation for finding the difference in phase between the ideal center frequency
		and the most prominant frequency content */
        void doHannRotate(std::vector<float>& inOut);
		// unrotate for passing the data to the output
        void doHannUnrotate(const std::vector<Cpx>& inCpx, std::vector<float>& outFloat);
        void phaseDiff2Hertz(std::vector<Polar<float> >& inOut);
        void hertzDiff2Phase(std::vector<Polar<float> >&);

    public:

		PVoc(int size, int hops, int offset, int sRate);

		//switched to `true' when the first PVoc is initialised. Upon init the static prev_Phase array is zeroed.
		static bool s_isInitialised;
		//The overlapping instances of STFT will all write their previous phase values to this static array.
    	static std::vector<float> s_prevPhase;

       virtual int setFFTSize(int newSize);
        virtual void process(const float* input, float* output, int blockSize);
        // still an abstract class that needs spectral_process to be overwritten
        virtual void spectral_process(const std::vector< Polar<float> > &in, std::vector<Polar<float> > &out, int bins)const = 0;
};

// the child classes will define the spectral process. At the moment they all derive from the STFT.

class frequencyShifter : public STFT {
    public:
        frequencyShifter(int size, int hops, int offset, int sRate) : STFT(size, hops, offset, sRate), m_shift(0){}
        
		virtual void spectral_process(const std::vector< Polar<float> > &in, std::vector<Polar<float> > &out, int bins)const;
		
		void setShift(float shift) { m_shift = (int)shift; }

	private: 
		int m_shift;
};

class frequencyMagnet : public STFT{
    public:
        frequencyMagnet(int size, int hops, int offset, int sRate) : STFT(size, hops, offset, sRate), m_freq(0.5), m_width(0.5), m_widthBias(0.5) {}
        virtual void spectral_process(const std::vector< Polar<float> > &in, std::vector<Polar<float> > &out, int bins)const;

		void setFrequency(float freq) { m_freq = freq; }
		void setWidth(float width) { m_width = width; }
		void setWidthBias(float widthBias) { m_widthBias = widthBias; };

	private:
		float m_freq;
		float m_width;
		float m_widthBias;


};

class spectralGate: public STFT{
    public:
        spectralGate(int size, int hops, int offset, int sRate) : STFT(size, hops, offset, sRate), m_cutOff(0.0), m_balance(0.0) {}
        
		virtual void spectral_process(const std::vector< Polar<float> > &in, std::vector<Polar<float> > &out, int bins)const;
		void setCutOff(float cutOff) { m_cutOff = cutOff; }
		void setBalance(float balance) { m_balance = balance; }
	private:
		float m_cutOff;
		float m_balance;

};

class sinusoidalShapedFilter : public STFT{
    public:
        sinusoidalShapedFilter(int size, int hops, int offset, int sRate) : STFT(size, hops, offset, sRate), m_freq(0.0), m_width(0.0), m_phase(0.0) {}
        virtual void spectral_process(const std::vector< Polar<float> > &in, std::vector<Polar<float> > &out, int bins)const;

		void setFrequency(float freq) { m_freq = freq; }
		void setWidth(float width) { m_width = width; }
		void setPhase(float phase) { m_phase = phase; }

	private:
		float m_freq;
		float m_width;
		float m_phase;

};

class binScrambler : public STFT{
  public:
		// previous scrambled index and new scrambled index. Will interpolate between the previous and the new
        std::vector<int> *m_AInd;
        std::vector<int> *m_BInd;

		void setPhase(int phase) { m_phase = phase; }
		void setMaxPhase(int maxPhase) { m_maxPhase = maxPhase; }

		// takes int vector pointer as arguments to the scrambled indices. The scrambling is done external to the spec proc
        binScrambler(int size, int hops, int offset, int sRate, std::vector<int> *A, std::vector<int> *B) : m_AInd(A), m_BInd(B), STFT(size, hops, offset, sRate), m_phase(0), m_maxPhase(sRate){}
		// swap the index pointers
        void swap(){ std::swap(m_AInd, m_BInd);}
        virtual void spectral_process(const std::vector< Polar<float> > &in, std::vector<Polar<float> > &out, int bins)const;

private:
	int m_phase;
	int m_maxPhase;
};

// for testing
class emptyProcess : public STFT{
    public:
        emptyProcess(int size, int hops, int offset, int sRate) : STFT(size, hops, offset, sRate) {}
        virtual void spectral_process(const std::vector< Polar<float> >& in, std::vector<Polar<float> >& out, int bins)const;
};
