#include "BaseAudioPlugin.h"

class Factory : public SpectralAudioPlugin::DependencyFactory {
public:
//    std::unique_ptr<ParameterContainerComponent> createUi(SpectralAudioPlugin *plugin) override {
    ParameterContainerComponent* createUi(SpectralAudioPlugin *plugin) override {
//        return std::make_unique<UiContainer>(m_baseParams, Colour::fromString(TEXT_COLOUR), 30);
        return new UiContainer(m_baseParams, Colour::fromString(TEXT_COLOUR), 30);
};
    
    std::shared_ptr<PluginParameters> createParams(SpectralAudioPlugin *plugin) override {
        m_baseParams = std::make_shared<BaseParameters>(plugin);
        return m_baseParams;
    };
    
    std::unique_ptr<SpectralAudioProcessorInteractor> createProcessor(SpectralAudioPlugin *plugin) override {
        return std::make_unique<BaseProcessor>(SpectralAudioPlugin::FFT_OVERLAPS, m_baseParams);
    }
private:
    std::shared_ptr<BaseParameters> m_baseParams;
};

// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SpectralAudioPlugin(new Factory());
}
