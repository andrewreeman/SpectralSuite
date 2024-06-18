#include "FrequencyShiftAudioPlugin.h"

class Factory : public SpectralAudioPlugin::DependencyFactory {
public:
    Factory() : m_pluginParams(nullptr) {
    }
    
    ~Factory(){}
    
    std::shared_ptr<PluginParameters> createParams(SpectralAudioPlugin* plugin) override {
        if(!m_pluginParams) {
            m_pluginParams = std::make_shared<FrequencyShiftPluginParameters>(plugin);
        }
        
        return m_pluginParams;
    }

    ParameterContainerComponent* createUi(SpectralAudioPlugin* plugin) override {
        return new FrequencySlider(m_pluginParams, Colour::fromString(TEXT_COLOUR), 30);
    }
    std::unique_ptr<SpectralAudioProcessorInteractor> createProcessor(SpectralAudioPlugin* plugin) override {
        return std::make_unique<FrequencyShiftInteractor>(SpectralAudioPlugin::FFT_OVERLAPS, m_pluginParams);
    };
    
    Array<int> fftSizesToNotInclude() override {
        return Array<int>(128, 256, 512);
    }
    
private:
    std::shared_ptr<FrequencyShiftPluginParameters> m_pluginParams;
};


// This creates new instances of the plugin..
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
