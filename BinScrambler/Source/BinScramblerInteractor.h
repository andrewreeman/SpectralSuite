#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../shared/StandardFFTProcessor.h"
#include "../../shared/PhaseBuffer.h"
#include "../../shared/SpectralAudioProcessorInteractor.h"
#include "BinScramblerParameters.h"

class BinScramblerInteractor : public SpectralAudioProcessorInteractor {
public:
    BinScramblerInteractor(int numOverlaps, std::shared_ptr<BinScramblerParameters> params);

	virtual void process(std::vector<std::vector<float>>* input, std::vector<std::vector<float>>* output) override;
	void prepareProcess(StandardFFTProcessor* spectralProcessor) override;
	std::unique_ptr<StandardFFTProcessor> createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps, int chans, int numChans) override;
	void onFftSizeChanged() override;	

private:
    float getFreq() const { return *m_rateParameter; };
    float getMaxPhase() const { return (int)( (float)getSampleRate() / getFreq()); };
    bool shouldRecalculateInternalParameters() const;
    
    void recalculateInternalParameters();
	void resetIndicies();
    void resetBIndicies();
	
	float* m_scrambleParameter;
	float* m_scatterParameter;
	float* m_rateParameter;
    
    float m_currentScrambleValue;
    float m_currentScatterValue;
    float m_currentRateValue;
		
	int m_phasor;

	// buffers for holding the previous and current indices.
	std::vector<int> m_A_Ind;
	std::vector<int> m_B_Ind;
    
	// pointers to the buffers that will be swapped on every new scramble
	std::vector<int>* m_pA_Ind;
	std::vector<int>* m_pB_Ind;
    
    int m_sprinkleAmount;
    int m_sprinkleRange;
    int m_scrambleFactor;
    int m_phasorMax;
};
