#include "FrequencyMagnetProcessor.h"

void FrequencyMagnetProcessor::createParameters(AudioProcessorValueTreeState* valueTreeState)
{			
	String freqHertzLabel = "Hz";
	valueTreeState->createAndAddParameter(std::make_unique<AudioParameterFloat>(
		"freq",
		"Frequency",
		NormalisableRange<float>(20.0f, 2000.0f), 800.0f, "",
		AudioProcessorParameter::Category::genericParameter,	
		[freqHertzLabel](float v, float maxLabelLength) { return String(v, 2) + freqHertzLabel; },
		[freqHertzLabel](String text) { return text.dropLastCharacters(freqHertzLabel.length()).getFloatValue(); }
	));

	m_freq = valueTreeState->getRawParameterValue("freq");	
	
	valueTreeState->createAndAddParameter(std::make_unique<AudioParameterFloat>(
		"bias",
		"Width bias",
		NormalisableRange<float>(0.0f, 1.0f), 0.01f, "",
		AudioProcessorParameter::Category::genericParameter		
		));

	m_bias = valueTreeState->getRawParameterValue("bias");

	valueTreeState->createAndAddParameter(std::make_unique<AudioParameterFloat>(
		"width",
		"Width",
		NormalisableRange<float>(0.0f, 1.0f), 0.5f, "",
		AudioProcessorParameter::Category::genericParameter
		));

	m_width = valueTreeState->getRawParameterValue("width");
}

void FrequencyMagnetProcessor::prepareProcess(int spectralProcessIndex)
{
	auto mag = (frequencyMagnet*)m_spectralProcess[spectralProcessIndex];
	mag->setFrequency(*m_freq);
	mag->setWidthBias(*m_bias);
	mag->setWidth(*m_width);
}

STFT * FrequencyMagnetProcessor::createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps)
{
	return new frequencyMagnet(fftSize, hopSize, hopSize * (index%numOverlaps), (int)sampleRate);
}