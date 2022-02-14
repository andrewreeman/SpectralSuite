#include "PlaygroundAudioPlugin.h"


class Factory : public SpectralAudioPlugin::DependencyFactory {
public:
    Factory() : m_params(nullptr){}
    
    ~Factory(){}
    
    std::shared_ptr<PluginParameters> createParams(SpectralAudioPlugin *plugin) override {
        if(!m_params) {
            m_params = std::make_shared<PlaygroundParameters>(plugin);
        }
        return m_params;
    }
    
    ParameterContainerComponent * createUi(SpectralAudioPlugin *plugin) override {
        return new UiContainer(m_params, Colour::fromString(TEXT_COLOUR), 30);
    }
    
    std::unique_ptr<SpectralAudioProcessorInteractor> createProcessor(SpectralAudioPlugin *plugin) override {
        return std::make_unique<PlaygroundProcessor>(SpectralAudioPlugin::FFT_OVERLAPS, m_params);
    }
    
private:
    std::shared_ptr<PlaygroundParameters> m_params;
};

// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SpectralAudioPlugin(new Factory());
}
