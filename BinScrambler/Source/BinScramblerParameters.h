#pragma once

#include "../../shared/PluginParameters.h"

class BinScramblerParameters : public PluginParameters, public AudioProcessorParameter::Listener {
public:
	BinScramblerParameters(AudioProcessor* processor);

	float* getScrambleValuePointer() const { return getRawParameterValue("scramble"); }
	float* getScatterValuePointer() const { return getRawParameterValue("scatter");	}
	float* getRateValuePointer() const { return getRawParameterValue("rate"); }

	AudioParameterInt* getSeedParameter() const { return (AudioParameterInt*)getParameter("seed"); }
	Value getSeedParameterAsValue() const { return this->getParameterAsValue("seed"); }
	unsigned int getSeed() const { return (unsigned int)getSeedParameter()->get(); }
	
	// AudioProcessorParameter listener methods
	void parameterValueChanged(int, float) override;
	void parameterGestureChanged(int, bool) override {};

private:
	void updateSRand();
	unsigned int m_lastRandomSeed;
};
