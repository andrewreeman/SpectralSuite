#pragma once

#include "JuceHeader.h"
#include "StandardFFTProcessor.h"
#include "FftSizeChoiceAdapter.h"
#include "FftStyleChoiceAdapter.h"
#include "FftOverlapsChoiceAdapter.h"
#include "FftWindowChoiceAdapter.h"
#include "FftSwitcher.h"
#include "VersionCheck.h"
#include "SpectralAudioProcessorInteractor.h"
#include "ParameterContainerComponentFactory.h"
#include "PluginParameters.h"

class SpectralAudioPluginUi;

class SpectralAudioPlugin : public AudioProcessor, private FftSwitcherThread::FftSwitcher
{
public:
	static const int FFT_OVERLAPS;
	static const int INIT_FFT_INDEX;

public:
    class DependencyFactory {
    public:
        virtual ~DependencyFactory(){};
        
        virtual std::shared_ptr<PluginParameters> createParams(SpectralAudioPlugin* plugin) = 0;
        virtual ParameterContainerComponent* createUi(SpectralAudioPlugin* plugin) = 0;
        virtual std::unique_ptr<SpectralAudioProcessorInteractor> createProcessor(SpectralAudioPlugin* plugin) = 0;
        virtual Array<int> fftSizesToNotInclude() { return Array<int>(); };
    };
    
//	class Dependencies {
//	public:
//		Dependencies(std::shared_ptr<PluginParameters> pluginParameters, std::unique_ptr<SpectralAudioProcessorInteractor> spectralAudioProcessor, std::unique_ptr<ParameterContainerComponent> parametersUi)
//			: parameters(pluginParameters), parametersUi(parametersUi.release()), spectralAudioProcessor(spectralAudioProcessor.release()) {}
//
//		std::shared_ptr<PluginParameters> getParams() { return parameters; }
//		std::unique_ptr<ParameterContainerComponent> moveUi() { return std::move(parametersUi); }
//		std::unique_ptr<SpectralAudioProcessorInteractor> moveProcessor() { return std::move(spectralAudioProcessor); }
//
//	private:
//		std::shared_ptr<PluginParameters> parameters;
//		std::unique_ptr<ParameterContainerComponent> parametersUi;
//		std::unique_ptr<SpectralAudioProcessorInteractor> spectralAudioProcessor;
//	};

//	typedef std::unique_ptr<Dependencies>(*DependencyCreator)(SpectralAudioPlugin*);

	SpectralAudioPlugin(
		DependencyFactory* dependencies
		//std::unique_ptr<SpectralAudioProcessor> audioProcessor, 
		//std::unique_ptr<ParameterContainerComponentFactory> parameterComponentFactory,		
	);
    ~SpectralAudioPlugin();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;		
    
	// FftSwitcher methods
    void switchFftSize() override;
    void switchFftStyle() override;
    void switchOverlapCount() override;
    void switchFftWindowType() override;

	void checkForUpdates(VersionCheckThread::Listener* onUpdateAvailableListener);

private:	
    //==============================================================================
	void emptyOutputs();
	void setFftSize(int fftSize);
    void initialiseParameters();
    bool isPreparingToPlay() const { return m_audioProcessorInteractor->isPreparingToPlay(); }
    bool isInvalidFftModificationState() const {
        return
            m_fftSwitcher.threadShouldExit()
            || m_audioProcessorInteractor->isPreparingToPlay()
            || m_audioProcessorInteractor->isPlaying()
            || m_output.empty()
            || m_input.empty();
    };
    
    std::shared_ptr<PluginParameters> parameters;
    std::unique_ptr<SpectralAudioProcessorInteractor> m_audioProcessorInteractor;

	FftSizeChoiceAdapter m_fftSizeChoiceAdapter;
    FftStyleChoiceAdapter m_fftStyleChoiceAdapter;
    FftOverlapsChoiceAdapter m_fftOverlapsChoiceAdapter;
    FftWindowChoiceAdapter m_fftWindowChoiceAdapter;
	FftSwitcherThread m_fftSwitcher;
	
	// io buffers: TODO n chan
	int m_internalBufferReadWriteIndex;
	std::vector<std::vector<float>> m_input;
	std::vector<std::vector<float>> m_output;
	
	//SpectralAudioPluginUi* m_ui;	
	VersionCheckThread m_versionCheckThread;
    std::unique_ptr<DependencyFactory> m_dependencyFactory;
 
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectralAudioPlugin)
};
