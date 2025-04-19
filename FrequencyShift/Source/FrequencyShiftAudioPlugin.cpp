#include "FrequencyShiftAudioPlugin.h"

class Factory : public SpectralAudioPlugin::DependencyFactory {
public:
    Factory() : m_pluginParams(nullptr) {
    }
    
    ~Factory() override = default;
    
    std::shared_ptr<PluginParameters> createParams(SpectralAudioPlugin* plugin) override {
        if(!m_pluginParams) {
            m_pluginParams = std::make_shared<FrequencyShiftPluginParameters>(plugin);
        }
        
        return m_pluginParams;
    }

    ParameterContainerComponent* createUi(SpectralAudioPlugin*) override {
        return new SliderContainer(m_pluginParams, Colour::fromString(TEXT_COLOUR), 30);
    }
    std::unique_ptr<SpectralAudioProcessorInteractor> createProcessor(SpectralAudioPlugin*) override {
        return std::make_unique<FrequencyShiftInteractor>(SpectralAudioPlugin::FFT_OVERLAPS, m_pluginParams);
    }
    
    Array<int> fftSizesToNotInclude() override {
        return Array{ 128, 256, 512 };
    }
    
private:
    std::shared_ptr<FrequencyShiftPluginParameters> m_pluginParams;
};


// This creates new instances of the plugin
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    // To debug AUval
//#if DEBUG
//    while (!Process::isRunningUnderDebugger())
//    {
//        Thread::sleep(250);
//    }
//#endif
    
	return new SpectralAudioPlugin(
		new Factory()
	);
}
