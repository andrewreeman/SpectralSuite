#include "SSFAudioPlugin.h"

// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{	
	SpectralAudioPlugin::DependencyCreator dependencyCreator = [](SpectralAudioPlugin* plugin) {
		auto pluginParams = std::make_shared<SSFParameters>(plugin);

		return std::make_unique<SpectralAudioPlugin::Dependencies>(
			pluginParams,
			std::make_unique<SSFProcessor>(SpectralAudioPlugin::FFT_OVERLAPS, SpectralAudioPlugin::N_CHANS),
			std::make_unique<SliderContainer>(pluginParams, Colour::fromString(TEXT_COLOUR), 30)
			);		
	};

    return new SpectralAudioPlugin(dependencyCreator);
}
