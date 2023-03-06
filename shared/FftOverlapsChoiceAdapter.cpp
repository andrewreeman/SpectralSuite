#include "FftOverlapsChoiceAdapter.h"

FftOverlapsChoiceAdapter::FftOverlapsChoiceAdapter(int initialIndex)
    : m_shouldChangeOverlap(false),
    m_currentIndex(initialIndex),
    m_parameter(nullptr)
{
    m_overlaps.add(1);
    m_overlaps.add(2);
    m_overlaps.add(4);
    m_overlaps.add(8);
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

void FftOverlapsChoiceAdapter::parameterValueChanged(int athing, float anotherthing)
{
    auto choiceIndex = m_parameter->getIndex();
    if (choiceIndex == m_currentIndex) { return; }
    m_currentIndex = choiceIndex;
    m_shouldChangeOverlap = true;
}
