/*
  ==============================================================================

    FftWindowChoiceAdapter.cpp
    Created: 19 Apr 2022 10:45:54am
    Author:  Andrew Reeman

  ==============================================================================
*/

#include "FftWindowChoiceAdapter.h"

FftWindowChoiceAdapter::FftWindowChoiceAdapter()
: m_shouldChangeFftWindow(false),
    m_currentWindow(FftWindowType::HANN),
    m_parameter(nullptr)
{
}

FftWindowType FftWindowChoiceAdapter::fftWindow()
{
    return m_currentWindow;
}

bool FftWindowChoiceAdapter::shouldChangeFftWindow()
{
    if (m_shouldChangeFftWindow) {
        m_shouldChangeFftWindow = false;
        return true;
    }
    else {
        return false;
    }
}

StringArray FftWindowChoiceAdapter::fftWindowStrings()
{
    StringArray strings;
    strings.add("Hann");
    strings.add("Blackman");
    strings.add("Hamming");
    strings.add("Blackman-Harris");

    return strings;
}

void FftWindowChoiceAdapter::listen(AudioParameterChoice * choiceParameter)
{
    choiceParameter->addListener(this);
    m_parameter = choiceParameter;
}

void FftWindowChoiceAdapter::parameterValueChanged(int, float)
{
    auto choiceIndex = m_parameter->getIndex();
    if(choiceIndex == 0) {
        m_currentWindow = FftWindowType::HANN;
    }
    else if(choiceIndex == 1) {
        m_currentWindow = FftWindowType::BLACKMAN;
    }
    else if(choiceIndex == 2) {
        m_currentWindow = FftWindowType::HAMMING;
    }
    else if(choiceIndex == 3) {
        m_currentWindow = FftWindowType::BLACKMAN_HARRIS;
    }
    else {
        m_currentWindow = FftWindowType::HANN;
    }

    m_shouldChangeFftWindow = true;
}

