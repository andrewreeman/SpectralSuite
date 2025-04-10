#include "PhaseLockInteractor.h"
#include "../../shared/SpectralAudioPlugin.h"
#include "../../shared/utilities.h"
#include "PhaseLockFFTProcessor.h"

std::unique_ptr<StandardFFTProcessor> PhaseLockInteractor::createSpectralProcess(int index, int fftSize, int hopSize,
    int sampleRate, int numOverlaps, int, int)
{    
	return std::make_unique<PhaseLockFFTProcessor>(fftSize, hopSize, hopSize * (index%numOverlaps), sampleRate, this->getPhaseBuffer());
}

void PhaseLockInteractor::process(SpectralAudioPlugin *plugin, std::vector<std::vector<float>> *input, std::vector<std::vector<float>> *output)
{
    if (m_params->willContinuePlayingWhenSilentInput())
    {
        const auto* playHead = plugin->getPlayHead();
        if(playHead != nullptr)
        {
            auto position = playHead->getPosition();
            if(position.hasValue()) 
            {
                if(!position->getIsPlaying())
                {
                    return;
                }
            }
        }
    }
    this->SpectralAudioProcessorInteractor::process(input, output);
}


void PhaseLockInteractor::prepareProcess(StandardFFTProcessor * spectralProcessor)
{
    auto processor = ((PhaseLockFFTProcessor*)spectralProcessor);
	if (m_params->isPhaseLocked()) {
		processor->lockPhase();
	}
	else {
		processor->unlockPhase();
	}
    
    if (m_params->isFreqLocked()) {
        processor->lockMag();
    }
    else {
        processor->unlockMag();
    }
            
    processor->setPhaseMix(*m_params->getPhaseMixParameter());
    processor->setMagMix(*m_params->getMagMixParameter());
    processor->setMagTrack(*m_params->getMagTrackParameter());
    processor->setRandomPhase(*m_params->getRandomPhaseParameter());
    
    processor->setMorphDurationSeconds(m_params->getMorphDurationParameter());
    if(m_params->shouldMorphMagAndPhase()) {
        processor->startMorphingMagAndFreq();
    }
    else {
        processor->stopMorphingMagAndFreq();
    }
}
