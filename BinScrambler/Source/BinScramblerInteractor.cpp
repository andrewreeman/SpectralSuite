#include "BinScramblerInteractor.h"

#include "../../shared/SpectralAudioPlugin.h"
#include "BinScramblerFFTProcessor.h"
#include "../../shared/utilities.h"

BinScramblerInteractor::BinScramblerInteractor(int numOverlaps, const std::shared_ptr<BinScramblerParameters> &params)
:  SpectralAudioProcessorInteractor(numOverlaps), m_currentScrambleValue(0.f), m_currentScatterValue(0.f), m_currentRateValue(0.f), m_phasor(0)
{
    m_pA_Ind = &m_A_Ind;
    m_pB_Ind = &m_B_Ind;
           
    m_scrambleParameter = params->getScrambleValuePointer();
    m_scatterParameter = params->getScatterValuePointer();
    m_rateParameter = params->getRateValuePointer();
    
    recalculateInternalParameters();
}

void BinScramblerInteractor::process(std::vector<std::vector<float>>* input, std::vector<std::vector<float>>* output)
{
	if (m_phasor >= m_phasorMax) {
		std::swap(m_pA_Ind, m_pB_Ind);
        resetBIndicies();
		
        if(shouldRecalculateInternalParameters()) {
            recalculateInternalParameters();
        }
        
        if(m_sprinkleAmount > 0.0) {
            utilities::vector_Low2HighDistribute(*m_pB_Ind, m_sprinkleAmount, m_sprinkleRange);
        }
		utilities::vector_Scramble(*m_pB_Ind, m_scrambleFactor);
		
        m_phasor -= m_phasorMax;

		for (auto& processChannel : m_spectralProcess) {
			for (auto& stftProcess : processChannel) {
				static_cast<BinScramblerFFTProcessor *>(stftProcess.get())->swap();
            }
		}		
	}
	
    m_phasor += getHopSize();

	SpectralAudioProcessorInteractor::process(input, output);	
}

void BinScramblerInteractor::prepareProcess(StandardFFTProcessor* spectralProcess)
{
	auto scrambler = static_cast<BinScramblerFFTProcessor *>(spectralProcess);
	scrambler->setPhase(m_phasor);
	scrambler->setMaxPhase(m_phasorMax);
}

std::unique_ptr<StandardFFTProcessor> BinScramblerInteractor::createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps, int, int)
{
	m_A_Ind.resize(static_cast<size_t>(fftSize / 2));
	m_B_Ind.resize(static_cast<size_t>(fftSize / 2));
	resetIndicies();
    recalculateInternalParameters();
    
	return std::make_unique<BinScramblerFFTProcessor>(fftSize, hopSize, hopSize * (index%numOverlaps), sampleRate, this->getPhaseBuffer(), &m_A_Ind, &m_B_Ind);
}

void BinScramblerInteractor::onFftSizeChanged()
{
	m_A_Ind.resize(static_cast<size_t>(getFftSize() / 2));
	m_B_Ind.resize(static_cast<size_t>(getFftSize() / 2));
	resetIndicies();
    recalculateInternalParameters();
}

void BinScramblerInteractor::resetIndicies()
{		
	for (size_t i = 0; i < m_A_Ind.size(); ++i) {
		m_A_Ind[i] = static_cast<int>(i);
		m_B_Ind[i] = static_cast<int>(i);
	}
}

void BinScramblerInteractor::recalculateInternalParameters() {
    m_currentRateValue = *m_rateParameter;
    m_currentScatterValue = *m_scatterParameter;
    m_currentScrambleValue = *m_scrambleParameter;
    
    m_phasorMax = static_cast<int>(getMaxPhase());
    const int binRange = getFftSize() / 2;

    // sprinkle amount determines the amount of lower indices to be mapped t
    const float scatter = *m_scatterParameter;
    const float scramble = *m_scrambleParameter;

    m_sprinkleAmount = static_cast<int>((1.f - (scatter*scatter))*100.f);
    m_sprinkleRange = binRange / 5;
    m_scrambleFactor = static_cast<int>(pow(scramble, 3.0f) * binRange) / 2;
}

void BinScramblerInteractor::resetBIndicies() {
    for (size_t i = 0; i < static_cast<size_t>(getFftSize()) / 2; ++i) m_pB_Ind->at(i) = static_cast<int>(i); // reset the b indices
}

bool BinScramblerInteractor::shouldRecalculateInternalParameters() const {
    return !exactlyEqual(m_currentRateValue, *m_rateParameter)
        || !exactlyEqual(m_currentScatterValue, *m_scatterParameter)
        || !exactlyEqual(m_currentScrambleValue, *m_scrambleParameter);
}
