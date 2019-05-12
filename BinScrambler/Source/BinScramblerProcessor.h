#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../shared/legacy/specprocess.h"
#include "../../shared/SpectralAudioProcessor.h"

class BinScramblerProcessor : public SpectralAudioProcessor {
public:
	BinScramblerProcessor(int numOverlaps, int numChans) : SpectralAudioProcessor(numOverlaps, numChans), m_scramble(nullptr), m_scatter(nullptr), m_rate(nullptr), m_Phase(0) {
		m_pA_Ind = &m_A_Ind;
		m_pB_Ind = &m_B_Ind;		
	}		
	
	void createParameters(PluginParameters* valueTreeState) override;
	void prepareProcess(int spectralProcessorIndex) override;
	STFT* createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps) override;		
	void onFftSizeChanged() override;	

private:	

	// simply fills the buffers holding index values with a range from 0 to fft size / 2.
	void refill_Indices();
	float getFreq();

	// parameters
	float* m_scramble;	
	float* m_scatter;
	float *m_rate;	
		
	int m_Phase;

	// buffers for holding the previous and current indices.
	std::vector<int> m_A_Ind;
	std::vector<int> m_B_Ind;
	// pointers that will be swapped on every new scramble
	std::vector<int> *m_pA_Ind;
	std::vector<int> *m_pB_Ind;
};
