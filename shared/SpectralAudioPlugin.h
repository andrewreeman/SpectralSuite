#pragma once

#include "JuceHeader.h"
#include "legacy/specprocess.h"
#include "FftChoiceAdapter.h"
#include "FftSwitcher.h"
#include "VersionCheck.h"
#include "SpectralAudioProcessor.h"
#include "ParameterContainerComponentFactory.h"
#include "PluginParameters.h"

//==============================================================================
/**
*/

class SpectralAudioPluginUi;

class SpectralAudioPlugin  : public AudioProcessor, private FftSwitcherThread::FftSwitcher
{
public:
	static const int FFT_OVERLAPS;
	static const int INIT_FFT_INDEX;
	static const int N_CHANS;

public:		
	class Dependencies {
	public:
		Dependencies(std::shared_ptr<PluginParameters> pluginParameters, std::unique_ptr<SpectralAudioProcessor> spectralAudioProcessor, std::unique_ptr<ParameterContainerComponent> parametersUi)
			: parameters(pluginParameters), parametersUi(parametersUi.release()), spectralAudioProcessor(spectralAudioProcessor.release()) {}

		std::shared_ptr<PluginParameters> getParams() { return parameters; }
		std::unique_ptr<ParameterContainerComponent> moveUi() { return std::move(parametersUi); }
		std::unique_ptr<SpectralAudioProcessor> moveProcessor() { return std::move(spectralAudioProcessor); }

	private:
		std::shared_ptr<PluginParameters> parameters;
		std::unique_ptr<ParameterContainerComponent> parametersUi;
		std::unique_ptr<SpectralAudioProcessor> spectralAudioProcessor;
	};

	typedef std::unique_ptr<Dependencies>(*DependencyCreator)(SpectralAudioPlugin*);

	SpectralAudioPlugin(
		DependencyCreator dependencyCreator,
		//std::unique_ptr<SpectralAudioProcessor> audioProcessor, 
		//std::unique_ptr<ParameterContainerComponentFactory> parameterComponentFactory,
		Array<int> fftSizes = Array<int>()
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

	void checkForUpdates(VersionCheckThread::Listener* onUpdateAvailableListener);

private:	
    //==============================================================================
	void emptyOutputs();
	void setFftSize(int fftSize);	
			
	std::shared_ptr<PluginParameters> parameters;
	std::unique_ptr<ParameterContainerComponent> m_parameterUiComponent;	
	std::unique_ptr<SpectralAudioProcessor> m_audioProcessor;

	FftChoiceAdapter m_fftChoiceAdapter;
	FftSwitcherThread m_fftSwitcher;
	
	std::unique_ptr<FileLogger> m_logger;
	
	// io buffers: TODO n chan
	int m_internalBufferReadWriteIndex;
	std::vector<float> m_output_L;
	std::vector<float> m_output_R;
	std::vector<float> m_Input_L;
	std::vector<float> m_Input_R;

	SpectralAudioPluginUi* m_ui;
	AudioValueTreeStateOnLoadListener* m_onLoadStateListener;

	VersionCheckThread m_versionCheckThread;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectralAudioPlugin)		
};
