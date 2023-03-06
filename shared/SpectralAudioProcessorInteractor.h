#pragma once

#include "JuceHeader.h"
#include "StandardFFTProcessor.h"
#include "PluginParameters.h"
#include "PhaseBuffer.h"

class SpectralAudioProcessorInteractor {
public:
    SpectralAudioProcessorInteractor(int numOverlaps);
    virtual ~SpectralAudioProcessorInteractor(){}

	int getHopSize() const { return m_fftHopSize; }	
	int getFftSize();
	int getSampleRate() const { return m_sampleRate; }
    int getNumOverlaps() const { return m_numOverlaps; }
    std::shared_ptr<PhaseBuffer> getPhaseBuffer() const { return m_phaseBuffer; }

	virtual std::unique_ptr<StandardFFTProcessor> createSpectralProcess(
        int index, int fftSize, int hopSize, int sampleRate, int numOverlaps, int channel, int channelCount
    ) = 0;
 
    virtual void receivedMidi(MidiBuffer& midiBuffer){};
	virtual void prepareProcess(StandardFFTProcessor*) {}	
	virtual void onFftSizeChanged() {};	
	
    virtual void process(std::vector<std::vector<float>>* input, std::vector<std::vector<float>>* output);
	
	void prepareToPlay(int fftSize, int sampleRate, int channelCount);			
	void setFftSize(int fftSize);
    void usePvoc(bool usePvoc) { m_phaseBuffer->setUsePvoc(usePvoc); };
    void setNumOverlaps(int newOverlapCount);

protected:	
	std::vector< std::vector<std::unique_ptr<StandardFFTProcessor>> > m_spectralProcess;

private:
    int m_numOverlaps;
	int m_sampleRate;
	int m_fftHopSize;
    int m_numChans;
    int m_fftSize;
    
    std::shared_ptr<PhaseBuffer> m_phaseBuffer;
};
