#include "BinScramblerAudioPlugin.h"


class Factory : public SpectralAudioPlugin::DependencyFactory {
public:
    Factory() : m_pluginParams(nullptr) {
    }
    
    ~Factory() override{}
    
    std::shared_ptr<PluginParameters> createParams(SpectralAudioPlugin* plugin) override {
        if(!m_pluginParams) {
            m_pluginParams = std::make_shared<BinScramblerParameters>(plugin);
        }
        
        return m_pluginParams;
    }

    ParameterContainerComponent* createUi(SpectralAudioPlugin*) override {
        return new SliderContainer(m_pluginParams, Colour::fromString(TEXT_COLOUR), 30);
    }
    std::unique_ptr<SpectralAudioProcessorInteractor> createProcessor(SpectralAudioPlugin*) override {
        return std::make_unique<BinScramblerInteractor>(SpectralAudioPlugin::FFT_OVERLAPS, m_pluginParams);
    }
private:
    std::shared_ptr<BinScramblerParameters> m_pluginParams;
};

// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SpectralAudioPlugin(new Factory());
}
