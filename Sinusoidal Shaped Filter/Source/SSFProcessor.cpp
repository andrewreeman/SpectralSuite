#include "SSFProcessor.h"

void SSFProcessor::createParameters(AudioProcessorValueTreeState* valueTreeState)
{			
	String freqHertzLabel = "Hz";
	valueTreeState->createAndAddParameter(std::make_unique<AudioParameterFloat>(
		"freq",
		"Frequency",
		NormalisableRange<float>(0.0f, 10.0f), 0.1f, "",
		AudioProcessorParameter::Category::genericParameter,	
		[freqHertzLabel](float v, float maxLabelLength) { return String(v, 2) + freqHertzLabel; },
		[freqHertzLabel](String text) { return text.dropLastCharacters(freqHertzLabel.length()).getFloatValue(); }
	));

	m_freq = valueTreeState->getRawParameterValue("freq");	

	String piLabel = " pi";
	valueTreeState->createAndAddParameter(std::make_unique<AudioParameterFloat>(
		"phase",
		"Phase",
		NormalisableRange<float>(0.0f, 2.0f), 0.01f, "",
		AudioProcessorParameter::Category::genericParameter,
		[piLabel](float v, float maxLabelLength) { return String(v, 2) + piLabel; },
		[piLabel](String text) { return text.dropLastCharacters(piLabel.length()).getFloatValue(); }
		));

	m_phase = valueTreeState->getRawParameterValue("phase");

	valueTreeState->createAndAddParameter(std::make_unique<AudioParameterFloat>(
		"width",
		"Width",
		NormalisableRange<float>(0.0f, 1.0f), 0.8f, "",
		AudioProcessorParameter::Category::genericParameter
		));

	m_width = valueTreeState->getRawParameterValue("width");
}

void SSFProcessor::prepareProcess(int spectralProcessIndex)
{
	auto ssf = (sinusoidalShapedFilter*)m_spectralProcess[spectralProcessIndex];
	ssf->setFrequency(*m_freq);
	ssf->setPhase(*m_phase);
	ssf->setWidth(*m_width);
}

STFT * SSFProcessor::createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps)
{
	return new sinusoidalShapedFilter(fftSize, hopSize, hopSize * (index%numOverlaps), (int)sampleRate);
}