#pragma once

#include "JuceHeader.h"
#include "legacy/specprocess.h"
#include "PluginParameters.h"

class SpectralAudioProcessor {
public:
	SpectralAudioProcessor(int numOverlaps)
		: m_numOverlaps(numOverlaps), m_sampleRate(48000), m_fftHopSize(0) {}
    
    virtual ~SpectralAudioProcessor(){}

	int getHopSize() { return m_fftHopSize; }	
	int getFftSize();
	int getSampleRate() { return m_sampleRate; }

	virtual std::unique_ptr<STFT> createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps) = 0;
	
	virtual void prepareProcess(STFT*) {}	
	virtual void onFftSizeChanged() {};	
	virtual void process(std::vector<std::vector<float>>* input, std::vector<std::vector<float>>* output);
	
	void prepareToPlay(int fftSize, int sampleRate, int channelCount);			
	void setFftSize(int fftSize);		

protected:	
	std::vector< std::vector<std::unique_ptr<STFT>> > m_spectralProcess;

private:
	const int m_numOverlaps;	

	int m_sampleRate;
	int m_fftHopSize;
};
