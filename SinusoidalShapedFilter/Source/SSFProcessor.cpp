#include "SSFProcessor.h"
//
//void SSFProcessor::createParameters(PluginParameters* valueTreeState)
//{				
//	m_freq = valueTreeState->getRawParameterValue("freq");		
//	m_phase = valueTreeState->getRawParameterValue("phase");	
//	m_width = valueTreeState->getRawParameterValue("width");
//}

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