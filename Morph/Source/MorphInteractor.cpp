#include "MorphInteractor.h"
#include "MorphFFTProcessor.h"
#include "../../shared//utilities.h"

void MorphInteractor::prepareProcess(StandardFFTProcessor* spectralProcessor)
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

std::unique_ptr<StandardFFTProcessor> MorphInteractor::createSpectralProcess(int index, int fftSize, int hopSize, int sampleRate, int numOverlaps, int chan, int numChans)
{
	return std::make_unique<MorphFFTProcessor>(fftSize, hopSize, hopSize * (index%numOverlaps), (int)sampleRate, this->getPhaseBuffer(), &pReadArray);
}

void MorphInteractor::controlPointsChanged(Array<float> controlPoints) {
    m_morphPointsChanged = false;
    
    auto fftSize = this->getFftSize() / 2; // we are only mapping the real components so ignoring imaginary
    auto controlPointsSize = controlPoints.size();
    
    if(fftSize == 0 || controlPointsSize == 0) { return; }
    if(fftSize == controlPointsSize) {
        
        Array<int> fftRangedControlPoints;
        for(int i=0; i<controlPointsSize; ++i) {
            fftRangedControlPoints.add(controlPoints[i] * fftSize);
        }
        
        *pWriteArray = fftRangedControlPoints;
    }
    else if(fftSize < controlPointsSize) {
        auto skip = fftSize / controlPointsSize;
        
        Array<int> compactedOutput;
        for(int i=0; i<controlPointsSize; i += skip) {
            float value = controlPoints[i];
            compactedOutput.add(value * fftSize);
        }
        
        *pWriteArray = compactedOutput;
    }
    else /* fftSize > controlPointSize */ {
        
        Array<int> expandedOutput;
        auto indexScale = (float)controlPointsSize / (float)fftSize;
        
        for(int i=0; i<fftSize; ++i) {
            float controlPointIndex = (float)i * indexScale;
            int indA = (int)controlPointIndex;
            int indB = indA + 1;
            
            float aValue = controlPoints[indA];
            if(indB >= controlPointsSize) {
                expandedOutput.add(aValue * fftSize);
            } else {
                float bValue = controlPoints[indB];
                float value = utilities::interp_lin(aValue, bValue, controlPointIndex);
                expandedOutput.add(value * fftSize);
            }
        }
        
        *pWriteArray = expandedOutput;
    }
    
    m_morphPointsChanged = true;
};
