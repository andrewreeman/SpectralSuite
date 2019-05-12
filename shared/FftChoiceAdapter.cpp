#include "FftChoiceAdapter.h"

FftChoiceAdapter::FftChoiceAdapter(int initialIndex) {
	for (int n = 7; n <= 14; n++) {
		m_ffts.add( (int)pow(2, n) );
	}

	m_currentIndex = initialIndex;
	m_shouldChangeFft = false;
}

int FftChoiceAdapter::fftSize()
{
	jassert(m_currentIndex < m_ffts.size());
	return m_ffts[m_currentIndex];
}

bool FftChoiceAdapter::shouldChangeFft()
{
	if (m_shouldChangeFft) {
		m_shouldChangeFft = false;
		return true;
	}
	else {
		return false;
	}
}

StringArray FftChoiceAdapter::fftStrings()
{
	StringArray strings;

	for (int fft : m_ffts) {
		strings.add(String(fft));
	}
	
	return strings;
}

void FftChoiceAdapter::listen(AudioParameterChoice * choiceParameter)
{
	choiceParameter->addListener(this);	
	m_parameter = choiceParameter;
}

void FftChoiceAdapter::remove(Array<int> fftSizes) {	
	if (fftSizes.isEmpty()) { return; }

	auto currentFftSize = fftSize();
	m_ffts.removeValuesIn(fftSizes);
	m_currentIndex = 0;
	
	// if current fft size was removed then default to first fft size
	int indexOfCurrentFftSize = m_ffts.indexOf(currentFftSize);
	if (indexOfCurrentFftSize!= -1) {
		m_currentIndex = indexOfCurrentFftSize;
	}
	else {
		m_currentIndex = 0;
	}
	
}

void FftChoiceAdapter::parameterValueChanged(int, float)
{		
	auto choiceIndex = m_parameter->getIndex();	
	if (choiceIndex == m_currentIndex) { return; }
	m_currentIndex = choiceIndex;
	m_shouldChangeFft = true;
}
