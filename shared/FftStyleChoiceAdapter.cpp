/*
  ==============================================================================

    FftStyleChoiceAdapter.cpp
    Created: 19 Apr 2022 10:45:54am
    Author:  Andrew Reeman

  ==============================================================================
*/

#include "FftStyleChoiceAdapter.h"

FftStyleChoiceAdapter::FftStyleChoiceAdapter()
: m_shouldChangeFftStyle(false),
    m_currentStyle(FftStyle::DEFAULT),
    m_parameter(nullptr)
{
}

FftStyle FftStyleChoiceAdapter::fftStyle()
{
    return m_currentStyle;
}

bool FftStyleChoiceAdapter::shouldChangeFftStyle()
{
    if (m_shouldChangeFftStyle) {
        m_shouldChangeFftStyle = false;
        return true;
    }
    else {
        return false;
    }
}

StringArray FftStyleChoiceAdapter::fftStyleStrings()
{
    StringArray strings;
    strings.add("Default");
    strings.add("Phase vocoder");

    return strings;
}

void FftStyleChoiceAdapter::listen(AudioParameterChoice * choiceParameter)
{
    choiceParameter->addListener(this);
    m_parameter = choiceParameter;
}

void FftStyleChoiceAdapter::parameterValueChanged(int, float)
{
    auto choiceIndex = m_parameter->getIndex();
    if(choiceIndex == 0) {
        m_currentStyle = FftStyle::DEFAULT;
    }
    else if(choiceIndex == 1) {
        m_currentStyle = FftStyle::PVOC;
    }
    else {
        m_currentStyle = FftStyle::DEFAULT;
    }

    m_shouldChangeFftStyle = true;
}

