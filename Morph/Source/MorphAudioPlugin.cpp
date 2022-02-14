#include "MorphAudioPlugin.h"

class Factory : public SpectralAudioPlugin::DependencyFactory {
public:
    std::shared_ptr<PluginParameters> createParams(SpectralAudioPlugin* plugin) override {
        if(m_params) {
            return m_params;
        }
        else {
            m_params = std::make_shared<MorphPluginParameters>(plugin);
            return m_params;
        }
    }
    
    ParameterContainerComponent * createUi(SpectralAudioPlugin *plugin) override {
        return new MorphSlider(m_params, Colour::fromString(TEXT_COLOUR), 30);
    }
    
    std::unique_ptr<SpectralAudioProcessorInteractor> createProcessor(SpectralAudioPlugin* plugin) override {
        return std::make_unique<MorphInteractor>(SpectralAudioPlugin::FFT_OVERLAPS, m_params);
    }
    
    Array<int> fftSizesToNotInclude() override {
        return Array<int>(128);
    }
    
private:
    std::shared_ptr<MorphPluginParameters> m_params;
};

// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new SpectralAudioPlugin(
		new Factory()
	);
}
