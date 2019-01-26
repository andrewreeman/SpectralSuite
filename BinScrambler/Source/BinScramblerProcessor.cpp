#include "BinScramblerProcessor.h"
#include "../../shared/SpectralAudioPlugin.h"
#include "../../shared/legacy/utilities.h"

void BinScramblerProcessor::createParameters(AudioProcessorValueTreeState* valueTreeState)
{				
	valueTreeState->createAndAddParameter(std::make_unique<AudioParameterFloat>(
		"scramble",
		"scramble",
		NormalisableRange<float>(0.0f, 1.0f), 0.1f, "",
		AudioProcessorParameter::Category::genericParameter		
	));

	m_scramble = valueTreeState->getRawParameterValue("scramble");	
	
	valueTreeState->createAndAddParameter(std::make_unique<AudioParameterFloat>(
		"scatter",
		"Scatter",
		NormalisableRange<float>(0.0f, 1.0f), 0.4f, "",
		AudioProcessorParameter::Category::genericParameter
		));

	m_scatter = valueTreeState->getRawParameterValue("scatter");	

	String freqHertzLabel = "Hz";
	valueTreeState->createAndAddParameter(std::make_unique<AudioParameterFloat>(
		"rate",
		"Rate",
		NormalisableRange<float>(0.25f, 15.0f), 2.0f, "",
		AudioProcessorParameter::Category::genericParameter,
		[freqHertzLabel](float v, float maxLabelLength) { return String(v, 2) + freqHertzLabel; },
		[freqHertzLabel](String text) { return text.dropLastCharacters(freqHertzLabel.length()).getFloatValue(); }
	));

	m_rate = valueTreeState->getRawParameterValue("rate");	
}

void BinScramblerProcessor::prepareProcess(int spectralProcessIndex)
{
	// This if statement is a hack to make this occur every time we need to proces audio. there really should be a different method for this

	float maxPhase = (getSampleRate() / getFreq()) * SpectralAudioPlugin::FFT_OVERLAPS;
	if (spectralProcessIndex == 0) {
		int binRange = getFftSize() / 2;
		// sprinkle amount determines the amount of lower indices to be mapped t
		float scatter = *m_scatter;
		float scramble = *m_scramble;		

		float sprinkleAmt = (1.f - (scatter*scatter))*100.f;
		int sprinkleRange = binRange / 5;
		int scramFac = (pow(scramble, 3)*binRange) / 2;		

		if (m_Phase >= maxPhase) {
			// when the phasor reaches its peak it will swap the bin pointer
			std::swap(m_pA_Ind, m_pB_Ind);
			for (int i = 0; i < binRange; ++i) m_pB_Ind->at(i) = i; // reset the b indices
			utilities::vector_Low2HighDistribute(*m_pB_Ind, int(sprinkleAmt), sprinkleRange);
			utilities::vector_Scramble(*m_pB_Ind, scramFac);
			m_Phase -= maxPhase; //wrap phase	

			for each (STFT* process in m_spectralProcess)
			{
				((binScrambler*)process)->swap();
			}
		}
		// increment the phasor
		m_Phase += getFftSize();
	}

	auto scrambler = (binScrambler*)m_spectralProcess[spectralProcessIndex];
	scrambler->setPhase(m_Phase);
	scrambler->setMaxPhase(maxPhase);	
}

STFT * BinScramblerProcessor::createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps)
{
	m_A_Ind.resize(fftSize / 2);
	m_B_Ind.resize(fftSize / 2);
	refill_Indices();
	return new binScrambler(fftSize, hopSize, hopSize * (index%numOverlaps), (int)sampleRate, &m_A_Ind, &m_B_Ind);
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
