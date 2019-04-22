#include "SpectralGateAudioPlugin.h"

class FrequencyShifterParameterContainerFactory : public ParameterComponentFactory  {
	EditorParameterContainer* create(AudioProcessorValueTreeState& valueTreeState) override {		
		return new SliderContainer(valueTreeState, Colour::fromString(TEXT_COLOUR), 30);
	}
	const int getComponentHeight() { return 160;  }
};
	
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SpectralAudioPlugin(
		std::make_unique<SpectralGateProcessor>(SpectralAudioPlugin::FFT_OVERLAPS, SpectralAudioPlugin::N_CHANS),
		std::make_unique<FrequencyShifterParameterContainerFactory>()
	);
}
