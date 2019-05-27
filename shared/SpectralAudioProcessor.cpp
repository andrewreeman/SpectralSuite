#include "SpectralAudioProcessor.h"

int SpectralAudioProcessor::getFftSize()
{
	return m_fftHopSize * m_numOverlaps;		
}

void SpectralAudioProcessor::prepareToPlay(int fftSize, int sampleRate, int channelCount)
{	
	m_sampleRate = sampleRate;
	m_fftHopSize = fftSize / m_numOverlaps; //SpectralAudioPlugin::FFT_OVERLAPS;
	m_spectralProcess.clear();
	
	for (int chan = 0; chan < channelCount; ++chan) {		 
		m_spectralProcess.push_back(std::vector<std::unique_ptr<STFT>>());
		
		for (int specProcess = 0; specProcess < m_numOverlaps; specProcess++) {
			m_spectralProcess.at(chan).push_back(createSpectralProcess(specProcess, fftSize, m_fftHopSize, sampleRate, m_numOverlaps));
		}			
	}
				
}

void SpectralAudioProcessor::process(const float* input, float* output, const float* inputR, float* outputR)
{	
	for (auto& spectralProcessChannel : m_spectralProcess) {
		for (auto& spectralProcess : spectralProcessChannel) {
			prepareProcess(spectralProcess.get());
			spectralProcess->process(&input[0], &output[0], m_fftHopSize);
		}

		//for (int i = 0; i < spectralProcessChannel.size(); i++) {
			//auto spectralProcess = spectralProcessChannel[i];
			//prepareProcess(spectralProcess);
			//spectralProcess->process(&input[0], &output[0], m_fftHopSize);
		//}
		//for (int i = 0; i < spectralProcessChannel.size(); i++) {
			//prepareProcess(i);
			//spectralProcessChannel[i]->process(&input[0], &output[0], m_fftHopSize);
		//}
	}

	//for (int i = 0; i < m_spectralProcess.size(); ++i) {
	//	prepareProcess(i);
	//	
	//	// This is a very ugly and inflexible way of doing stereo processing
	//	if (i < m_numOverlaps) {
	//		m_spectralProcess[i]->process(&input[0], &output[0], m_fftHopSize);
	//	}
	//	else {
	//		m_spectralProcess[i]->process(&inputR[0], &outputR[0], m_fftHopSize);
	//	}
	//}
}

void SpectralAudioProcessor::setFftSize(int fftSize)
{
	m_fftHopSize = fftSize / m_numOverlaps;
	for (auto& spectralProcessChannel : m_spectralProcess) {
		for(int i=0; i<spectralProcessChannel.size(); ++i) {			
			
			if (fftSize == spectralProcessChannel[i]->getFFTSize()) { continue; }

			spectralProcessChannel[i]->setFFTSize(fftSize);
			spectralProcessChannel[i]->setHopSize(m_fftHopSize);
			spectralProcessChannel[i]->setOffset(m_fftHopSize * (i%m_numOverlaps));
		}
	}

	/*for (int i = 0; i < m_spectralProcess.size(); i++) {
		auto spectralProcess = m_spectralProcess[i];
		if (fftSize == spectralProcess->getFFTSize()) { continue; }

		spectralProcess->setFFTSize(fftSize);
		spectralProcess->setHopSize(m_fftHopSize);
		spectralProcess->setOffset(m_fftHopSize * (i%m_numOverlaps));
	}*/

	this->onFftSizeChanged();
}
