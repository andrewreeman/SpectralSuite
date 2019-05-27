#pragma once

#include "JuceHeader.h"
#include "legacy/specprocess.h"
#include "PluginParameters.h"

class SpectralAudioProcessor {
public:
	SpectralAudioProcessor(int numOverlaps, int numChans)
		: m_numOverlaps(numOverlaps), m_fftHopSize(0) {}

	int getHopSize() { return m_fftHopSize; }	
	int getFftSize();
	int getSampleRate() { return m_sampleRate; }

	virtual std::unique_ptr<STFT> createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps) = 0;
//	virtual void createParameters(PluginParameters*) {};

	// Arguments:
	//	int: index of the spectral processor to prepare
	virtual void prepareProcess(STFT*) {}	
	virtual void onFftSizeChanged() {};
	virtual void process(const float* input, float* output, const float* inputR, float* outputR);
	
	void prepareToPlay(int fftSize, int sampleRate, int channelCount);			
	void setFftSize(int fftSize);		

protected:	
	std::vector< std::vector<std::unique_ptr<STFT>> > m_spectralProcess;

private:
	const int m_numOverlaps;	

	int m_sampleRate;
	int m_fftHopSize;
};
