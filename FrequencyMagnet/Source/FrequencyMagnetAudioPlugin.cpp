#include "FrequencyMagnetAudioPlugin.h"

// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	//create shared pointer here and pass to processor and factory...which should actually just be the container
	SpectralAudioPlugin::DependencyCreator dependencyCreator = [](SpectralAudioPlugin* plugin) {
		auto pluginParams = std::make_shared<FrequencyMagnetParameters>(plugin);

		return std::make_unique<SpectralAudioPlugin::Dependencies>(
			pluginParams,
			std::make_unique<FrequencyMagnetProcessor>(SpectralAudioPlugin::FFT_OVERLAPS, SpectralAudioPlugin::N_CHANS, pluginParams),
			std::make_unique<SliderContainer>(pluginParams, Colour::fromString(TEXT_COLOUR), 30)
			);		
	};

    return new SpectralAudioPlugin(dependencyCreator);
}
