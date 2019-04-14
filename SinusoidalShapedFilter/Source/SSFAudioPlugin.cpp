#include "SSFAudioPlugin.h"

class FrequencyShifterParameterContainerFactory : public ParameterComponentFactory  {
	Component* create(AudioProcessorValueTreeState& valueTreeState) override {		
		return new SliderContainer(valueTreeState, Colour::fromString(TEXT_COLOUR), 30);
	}
	const int getComponentHeight() { return 240;  }
};
	
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SpectralAudioPlugin(
		std::make_unique<SSFProcessor>(SpectralAudioPlugin::FFT_OVERLAPS, SpectralAudioPlugin::N_CHANS),
		std::make_unique<FrequencyShifterParameterContainerFactory>()
	);
}