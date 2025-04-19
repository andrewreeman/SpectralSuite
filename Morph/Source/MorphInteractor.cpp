#include "MorphInteractor.h"
#include "MorphFFTProcessor.h"
#include "../../shared//utilities.h"

void MorphInteractor::prepareProcess(StandardFFTProcessor*)
{
    if(m_morphPointsChanged) {
        m_morphPointsChanged = false;
        std::swap(pReadArray, pWriteArray);
    }
    // TODO: pre calc half size
    else if(pReadArray->size() != (this->getFftSize() / 2 ))
    {
        m_morphParams->triggerControlPointsChanged();
    }
}

std::unique_ptr<StandardFFTProcessor> MorphInteractor::createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps, int, int)
{
	return std::make_unique<MorphFFTProcessor>(fftSize, hopSize, hopSize * (index%numOverlaps), sampleRate, this->getPhaseBuffer(), &pReadArray);
}

void MorphInteractor::controlPointsChanged(Array<float> controlPoints) {
    m_morphPointsChanged = false;

    const auto fftSize = this->getFftSize() / 2; // we are only mapping the real components so ignoring imaginary
    const auto controlPointsSize = controlPoints.size();
    
    if(fftSize == 0 || controlPointsSize == 0) { return; }
    if(fftSize == controlPointsSize) {
        
        Array<int> fftRangedControlPoints;
        for(int i=0; i < controlPointsSize; ++i) {
            fftRangedControlPoints.add(static_cast<int>(controlPoints[i] * static_cast<float>(fftSize)));
        }
        
        *pWriteArray = fftRangedControlPoints;
    }
    else if(fftSize < controlPointsSize) {
        const auto skip = fftSize / controlPointsSize;
        
        Array<int> compactedOutput;
        for(int i=0; i<controlPointsSize; i += skip) {
            const float value = controlPoints[i];
            compactedOutput.add(static_cast<int>(value * static_cast<float>(fftSize)));
        }
        
        *pWriteArray = compactedOutput;
    }
    else /* fftSize > controlPointSize */ {
        
        Array<int> expandedOutput;
        const auto indexScale = static_cast<float>(controlPointsSize) / static_cast<float>(fftSize);
        
        for(int i=0; i<fftSize; ++i) {
            const float controlPointIndex = static_cast<float>(i) * indexScale;
            const int indA = static_cast<int>(controlPointIndex);
            const int indB = indA + 1;

            const float aValue = controlPoints[indA];
            if(indB >= controlPointsSize) {
                expandedOutput.add(static_cast<int>(aValue * static_cast<float>(fftSize)));
            } else {
                const float bValue = controlPoints[indB];
                const float value = utilities::interp_lin(aValue, bValue, controlPointIndex);
                expandedOutput.add(static_cast<int>(value * static_cast<float>(fftSize)));
            }
        }
        
        *pWriteArray = expandedOutput;
    }
    
    m_morphPointsChanged = true;
}
