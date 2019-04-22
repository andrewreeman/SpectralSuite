#pragma once

#include "JuceHeader.h"
#include "legacy/specprocess.h"

class SpectralAudioProcessor {
public:
	SpectralAudioProcessor(int numOverlaps, int numChans)
		: m_numOverlaps(numOverlaps),
		m_numChans(numChans), m_fftHopSize(0) {}

	int getHopSize() { return m_fftHopSize; }	
	int getFftSize();
	int getSampleRate() { return m_sampleRate; }

	virtual STFT* createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps) = 0;
	virtual void createParameters(AudioProcessorValueTreeState* valueTreeState) {};
	virtual void prepareProcess(int spectralProcessorIndex) {}	
	virtual void onFftSizeChanged() {};
	virtual void process(const float* input, float* output, const float* inputR, float* outputR);
	
	void prepareToPlay(int fftSize, int sampleRate);			
	void setFftSize(int fftSize);		

protected:	
	OwnedArray<STFT> m_spectralProcess;

private:
	const int m_numOverlaps;
	const int m_numChans;	

	int m_sampleRate;
	int m_fftHopSize;
};
