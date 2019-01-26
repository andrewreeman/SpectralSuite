#include "SpectralGateProcessor.h"

void SpectralGateProcessor::createParameters(AudioProcessorValueTreeState* valueTreeState)
{			
	valueTreeState->createAndAddParameter(std::make_unique<AudioParameterFloat>(
		"cutoff",
		"Cutoff",
		NormalisableRange<float>(0.0f, 1.0f), 0.6f, "",
		AudioProcessorParameter::Category::genericParameter		
	));

	m_cutOff = valueTreeState->getRawParameterValue("cutoff");	

	valueTreeState->createAndAddParameter(std::make_unique<AudioParameterFloat>(
		"balance",
		"Weak/Strong Balance",
		NormalisableRange<float>(0.0f, 1.0f), 0.7f, "",
		AudioProcessorParameter::Category::genericParameter
		));

	m_balance = valueTreeState->getRawParameterValue("balance");
}

void SpectralGateProcessor::prepareProcess(int spectralProcessIndex)
{
	auto gate = (spectralGate*)m_spectralProcess[spectralProcessIndex];	
	gate->setCutOff(*m_cutOff);
	gate->setBalance(*m_balance);
}

STFT * SpectralGateProcessor::createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps)
{
	return new spectralGate(fftSize, hopSize, hopSize * (index%numOverlaps), (int)sampleRate);
}