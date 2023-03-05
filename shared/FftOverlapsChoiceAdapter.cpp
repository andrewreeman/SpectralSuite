#include "FftOverlapsChoiceAdapter.h"

FftOverlapsChoiceAdapter::FftOverlapsChoiceAdapter(int initialIndex)
    : m_shouldChangeOverlap(false),
    m_currentIndex(initialIndex),
    m_parameter(nullptr)
{
    for (int n = 1; n <= 8; n++) {
        m_overlaps.add(n);
    }
}

int FftOverlapsChoiceAdapter::overlapCount()
{
    jassert(m_currentIndex < m_overlaps.size());
    return m_overlaps[m_currentIndex];
}

bool FftOverlapsChoiceAdapter::shouldChangeFftOverlaps()
{
    if (m_shouldChangeOverlap) {
        m_shouldChangeOverlap = false;
        return true;
    }
    else {
        return false;
    }
}

StringArray FftOverlapsChoiceAdapter::overlapStrings()
{
    StringArray strings;

    for (int overlap : m_overlaps) {
        strings.add(String(overlap));
    }
    
    return strings;
}

void FftOverlapsChoiceAdapter::listen(AudioParameterChoice * choiceParameter)
{
    choiceParameter->addListener(this);
    m_parameter = choiceParameter;
}

void FftOverlapsChoiceAdapter::parameterValueChanged(int, float)
{
    auto choiceIndex = m_parameter->getIndex();
    if (choiceIndex == m_currentIndex) { return; }
    m_currentIndex = choiceIndex;
    m_shouldChangeOverlap = true;
}
