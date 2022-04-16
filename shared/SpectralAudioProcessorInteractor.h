#pragma once

#include "JuceHeader.h"
#include "StandardFFTProcessor.h"
#include "PluginParameters.h"

class SpectralAudioProcessorInteractor {
public:
	SpectralAudioProcessorInteractor(int numOverlaps)
		: m_numOverlaps(numOverlaps), m_sampleRate(48000), m_fftHopSize(0), m_numChans(2) {}
    
    virtual ~SpectralAudioProcessorInteractor(){}

	int getHopSize() const { return m_fftHopSize; }	
	int getFftSize();
	int getSampleRate() const { return m_sampleRate; }
    int getNumOverlaps() const { return m_numOverlaps; }

	virtual std::unique_ptr<STFT> createSpectralProcess(
        int index, int fftSize, int hopSize, int sampleRate, int numOverlaps, int channel, int channelCount
    ) = 0;
 
    // Get the overlap count from the UI then call 'setOverlapCount'
    virtual void switchOverlapCount(){};
	
    virtual void receivedMidi(MidiBuffer& midiBuffer){};
	virtual void prepareProcess(STFT*) {}	
	virtual void onFftSizeChanged() {};	
	
    virtual void process(std::vector<std::vector<float>>* input, std::vector<std::vector<float>>* output);
	
	void prepareToPlay(int fftSize, int sampleRate, int channelCount);			
	void setFftSize(int fftSize);
    void setNumOverlaps(int newOverlapCount);        

protected:	
	std::vector< std::vector<std::unique_ptr<STFT>> > m_spectralProcess;

private:
    int m_numOverlaps;
	int m_sampleRate;
	int m_fftHopSize;
    int m_numChans;
    int m_fftSize;
};
