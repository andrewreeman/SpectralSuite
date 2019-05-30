#include "BinScramblerProcessor.h"
#include "../../shared/SpectralAudioPlugin.h"
#include "../../shared/legacy/utilities.h"

void BinScramblerProcessor::process(std::vector<std::vector<float>>* input, std::vector<std::vector<float>>* output)
{
	const int maxPhase = (getSampleRate() / (int)getFreq()) * SpectralAudioPlugin::FFT_OVERLAPS;
	if (m_Phase >= maxPhase) {
		const int binRange = getFftSize() / 2;

		// sprinkle amount determines the amount of lower indices to be mapped t
		const float scatter = *m_scatter;
		const float scramble = *m_scramble;

		const float sprinkleAmt = (1.f - (scatter*scatter))*100.f;
		const int sprinkleRange = binRange / 5;
		const int scramFac = (int)(pow(scramble, 3)*binRange) / 2;

		// when the phasor reaches its peak it will swap the bin pointer
		std::swap(m_pA_Ind, m_pB_Ind);
		for (int i = 0; i < binRange; ++i) m_pB_Ind->at(i) = i; // reset the b indices
		utilities::vector_Low2HighDistribute(*m_pB_Ind, int(sprinkleAmt), sprinkleRange);
		utilities::vector_Scramble(*m_pB_Ind, scramFac);
		m_Phase -= maxPhase; //wrap phase	

		for (auto& processChannel : m_spectralProcess) {
			for (auto& stftProcess : processChannel) {
				((binScrambler*)stftProcess.get())->swap();
			}
		}		
	}
	
	m_Phase += getFftSize();

	SpectralAudioProcessor::process(input, output);	
}

void BinScramblerProcessor::prepareProcess(STFT* spectralProcess)
{		
	const int maxPhase = (getSampleRate() / (int)getFreq()) * SpectralAudioPlugin::FFT_OVERLAPS;
	auto scrambler = (binScrambler*)spectralProcess;
	scrambler->setPhase(m_Phase);
	scrambler->setMaxPhase(maxPhase);	
}

std::unique_ptr<STFT> BinScramblerProcessor::createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps)
{
	m_A_Ind.resize(fftSize / 2);
	m_B_Ind.resize(fftSize / 2);
	refill_Indices();
	return std::make_unique<binScrambler>(fftSize, hopSize, hopSize * (index%numOverlaps), (int)sampleRate, &m_A_Ind, &m_B_Ind);
}

void BinScramblerProcessor::onFftSizeChanged()
{
	m_A_Ind.resize(getFftSize() / 2);
	m_B_Ind.resize(getFftSize() / 2);
	refill_Indices();
}

void BinScramblerProcessor::refill_Indices()
{		
	for (int i = 0; i < m_A_Ind.size(); ++i) {
		m_A_Ind[i] = i;
		m_B_Ind[i] = i;
	}
}

float BinScramblerProcessor::getFreq() {
	return *m_rate;
	/*float rate = *m_rate;	
	return rate * (getSampleRate() / getHopSize());	*/
}
