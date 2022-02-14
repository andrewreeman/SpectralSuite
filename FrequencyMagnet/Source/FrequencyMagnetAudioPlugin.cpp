#include "FrequencyMagnetAudioPlugin.h"

class Factory : public SpectralAudioPlugin::DependencyFactory {
public:
    Factory() : m_pluginParams(nullptr) {
    }
    
    ~Factory(){}
    
    std::shared_ptr<PluginParameters> createParams(SpectralAudioPlugin* plugin) override {
        if(!m_pluginParams) {
            m_pluginParams = std::make_shared<FrequencyMagnetParameters>(plugin);
        }
        
        return m_pluginParams;
    }

    ParameterContainerComponent* createUi(SpectralAudioPlugin* plugin) override {
        return new SliderContainer(m_pluginParams, Colour::fromString(TEXT_COLOUR), 30);
    }
    std::unique_ptr<SpectralAudioProcessorInteractor> createProcessor(SpectralAudioPlugin* plugin) override {
        return std::make_unique<FrequencyMagnetInteractor>(SpectralAudioPlugin::FFT_OVERLAPS, m_pluginParams);
    };
private:
    std::shared_ptr<FrequencyMagnetParameters> m_pluginParams;
};


// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SpectralAudioPlugin(new Factory());
}
