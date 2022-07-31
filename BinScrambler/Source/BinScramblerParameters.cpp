#include "BinScramblerParameters.h"

BinScramblerParameters::BinScramblerParameters(AudioProcessor * processor) : 
	PluginParameters(processor),
	m_lastRandomSeed(1)
{
	createAndAddParameter(std::make_unique<AudioParameterFloat>(
		ParameterID("scramble", 1),
		"Scramble",
		NormalisableRange<float>(0.0f, 1.0f), 0.1f, "",
		AudioProcessorParameter::Category::genericParameter
		));

	createAndAddParameter(std::make_unique<AudioParameterFloat>(
		ParameterID("scatter", 1),
		"Scatter",
		NormalisableRange<float>(0.0f, 1.0f), 0.4f, "",
		AudioProcessorParameter::Category::genericParameter
		));

	String freqHertzLabel = "Hz";
	createAndAddParameter(std::make_unique<AudioParameterFloat>(
		ParameterID("rate", 1),
		"Rate",
		NormalisableRange<float>(0.25f, 15.0f), 2.0f, "",
		AudioProcessorParameter::Category::genericParameter,
		[freqHertzLabel](float v, int) { return String(v, 2) + freqHertzLabel; },
		[freqHertzLabel](const String& text) { return text.dropLastCharacters(freqHertzLabel.length()).getFloatValue(); }
	));

	createAndAddParameter(std::make_unique<AudioParameterInt>(
		ParameterID("seed", 1),
		"Random seed",
		1, 
		9999, 
		0
	));
	
	getParameter("seed")->addListener(this);
}

void BinScramblerParameters::parameterValueChanged(int, float)
{	
	updateSRand();
}

void BinScramblerParameters::updateSRand()
{
	unsigned int seed = getSeed();
	if (seed == m_lastRandomSeed) { return; }
	m_lastRandomSeed = seed;

	if (seed > 0) {
		std::srand(seed);
	}
	else {
		std::srand((unsigned int)time(NULL));
	}
}
