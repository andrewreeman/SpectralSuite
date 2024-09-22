#include "SpectralAudioPlugin.h"

// avoiding circular declaration
#include "SpectralAudioPluginUi.h"

// TODO: should be index instead
const int SpectralAudioPlugin::FFT_OVERLAPS = 4;
const int SpectralAudioPlugin::INIT_FFT_INDEX = 4; // 2048 

//==============================================================================
SpectralAudioPlugin::SpectralAudioPlugin(
	//std::unique_ptr<SpectralAudioProcessor> audioProcessor, std::unique_ptr<ParameterContainerComponentFactory> parameterComponentFactory
	DependencyFactory* dependencies
//	Array<int> fftSizesToRemove
)
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", AudioChannelSet::stereo(), true)
#endif
	),

#endif
	m_fftSizeChoiceAdapter(INIT_FFT_INDEX),
	//parameters(*this, nullptr),

    m_fftOverlapsChoiceAdapter(1),
	m_fftSwitcher(this),
    m_internalBufferReadWriteIndex(0),
	m_versionCheckThread(VersionCode, "https://www.andrewreeman.com/spectral_suite_publish.json"),
    m_dependencyFactory(dependencies)
{
    this->initialiseParameters();
}

SpectralAudioPlugin::~SpectralAudioPlugin()
{    
    if(this->m_versionCheckThread.isThreadRunning()) {
        this->m_versionCheckThread.stopThread(20);
    }
    
    if(this->m_fftSwitcher.isThreadRunning()) {
        this->m_fftSwitcher.stopThread(20);
    }
}

/* FFT Switcher methods */
void SpectralAudioPlugin::switchFftSize()
{
    if (isInvalidFftModificationState()) { return; }
//    if (m_audioProcessorInteractor->isPreparingToPlay()) { return; }

	setFftSize(m_fftSizeChoiceAdapter.fftSize());
    
    if(getActiveEditor() != nullptr) {
        SpectralAudioPluginUi* ui = (SpectralAudioPluginUi*)getActiveEditor();
        ui->onFftSizeChanged();
    }
}
void SpectralAudioPlugin::switchFftStyle()
{
    if (isInvalidFftModificationState()) { return; }

    FftStyle style = m_fftStyleChoiceAdapter.fftStyle();
    switch(style) {
        case FftStyle::DEFAULT:
            m_audioProcessorInteractor->usePvoc(false);
            break;
        case FftStyle::PVOC:
            m_audioProcessorInteractor->usePvoc(true);
            break;
        default:
            break;
    }
    
    if(getActiveEditor() != nullptr) {
        SpectralAudioPluginUi* ui = (SpectralAudioPluginUi*)getActiveEditor();
        ui->onFftStyleChanged();
    }
}

void SpectralAudioPlugin::switchOverlapCount() {
    if (isInvalidFftModificationState()) { return; }

    int overlaps = m_fftOverlapsChoiceAdapter.overlapCount();
    m_audioProcessorInteractor->setNumOverlaps(overlaps);
    
    const int hopSize = m_audioProcessorInteractor->getHopSize();
    
    for(std::vector<float>& output : m_output)
    {
        if (output.size() == hopSize)
        {
            continue;
        }
        
        if (isInvalidFftModificationState()) { return; }
        // Because we switch overlaps on a different thread m_output might be empty when we resize
        // Which means we no longer own the output vector
        // output vector expected to only be empty if we have been destructed
        // TODO: use mutex when modifying fft buffers
        if (!m_output.empty() && !output.empty())
        {
            output.resize(hopSize, 0.f);
        }
    }
    
    for(std::vector<float>& input : m_input)
    {
        if (input.size() == hopSize)
        {
            continue;
        }
 
        if (isInvalidFftModificationState()) { return; }
        if (!m_input.empty() && !input.empty())
        {
            input.resize(hopSize, 0.f);
        }
    }
    
	setLatencySamples(m_audioProcessorInteractor->getFftSize() + hopSize);
}


void SpectralAudioPlugin::switchFftWindowType() {
    if (isInvalidFftModificationState()) { return; }
    
    auto windowType = m_fftWindowChoiceAdapter.fftWindow();
    m_audioProcessorInteractor->setWindowType(windowType);
}

/* Public methods */

const String SpectralAudioPlugin::getName() const
{
    return JucePlugin_Name;
}

bool SpectralAudioPlugin::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SpectralAudioPlugin::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SpectralAudioPlugin::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SpectralAudioPlugin::getTailLengthSeconds() const
{
    return 0.0;
}

int SpectralAudioPlugin::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SpectralAudioPlugin::getCurrentProgram()
{
    return 0;
}

void SpectralAudioPlugin::setCurrentProgram (int)
{
}

const String SpectralAudioPlugin::getProgramName (int)
{
    return {};
}

void SpectralAudioPlugin::changeProgramName (int, const String&)
{
}

void SpectralAudioPlugin::prepareToPlay (double sampleRate, int)
{    
    int waitCount = 0;
    while (m_fftSwitcher.isBusy() && waitCount < 100)
    {
        waitCount++;
        Thread::sleep(100);
    }
    
    if (m_fftSwitcher.isBusy())
    {
        return;
    }
    
	m_output.clear();
	m_input.clear();
    
	for (
		int outputChannelCount = getBusesLayout().getMainOutputChannels();		
		outputChannelCount > 0;
		outputChannelCount--
	)
	{
		m_output.push_back(std::vector<float>());
		m_input.push_back(std::vector<float>());
	}
    
    const int fftSize = m_fftSizeChoiceAdapter.fftSize();
	m_audioProcessorInteractor->prepareToPlay(fftSize, (int)sampleRate, getBusesLayout().getMainOutputChannels());
    
	setFftSize(fftSize);
}

void SpectralAudioPlugin::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SpectralAudioPlugin::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else  
    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SpectralAudioPlugin::emptyOutputs() {
	for(auto& output : m_output)
	{
		std::fill(output.begin(), output.end(), 0.f);
	}	
}

void SpectralAudioPlugin::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiBuffer)
{
	if (m_fftSwitcher.isBusy() || m_audioProcessorInteractor->isPreparingToPlay()) {
		m_internalBufferReadWriteIndex = 0;
		return;
	}

	if (m_fftSizeChoiceAdapter.shouldChangeFftSize()) {
		m_fftSwitcher.switchFftSize();
		return;
	}
 
    if(m_fftOverlapsChoiceAdapter.shouldChangeFftOverlaps()) {
        m_fftSwitcher.switchOverlapCount();
        return;
    }
    
    if(m_fftStyleChoiceAdapter.shouldChangeFftStyle()) {
        m_fftSwitcher.switchFftStyle();
        return;
    }
    
    if(m_fftWindowChoiceAdapter.shouldChangeFftWindow()) {
        m_fftSwitcher.switchWindowType();
        return;
    }
    
    if(!midiBuffer.isEmpty()) {
        m_audioProcessorInteractor->receivedMidi(midiBuffer);
    }        

	const int hopSize = m_audioProcessorInteractor->getHopSize();
	const int numChannels = buffer.getNumChannels();
	auto audio = buffer.getArrayOfWritePointers();
	int numSamples = buffer.getNumSamples();
	int ioVSTBuffers = 0;

	while (numSamples--) {		
		if (m_internalBufferReadWriteIndex >= hopSize) {
			m_internalBufferReadWriteIndex = 0;
			emptyOutputs();
			m_audioProcessorInteractor->process(this, &m_input, &m_output);
		}

		for (int channel = 0; channel < numChannels; channel++)
		{
			m_input[channel][m_internalBufferReadWriteIndex] = audio[channel][ioVSTBuffers];
			audio[channel][ioVSTBuffers] = m_output[channel][m_internalBufferReadWriteIndex];
		}
		
		ioVSTBuffers++;
		m_internalBufferReadWriteIndex++;	
	}			
}

bool SpectralAudioPlugin::hasEditor() const
{
    return true;
}

AudioProcessorEditor* SpectralAudioPlugin::createEditor()
{
    auto uiComponent = m_dependencyFactory->createUi(this);
	return new SpectralAudioPluginUi(*this, parameters.get(), uiComponent);
}

//==============================================================================
void SpectralAudioPlugin::getStateInformation (MemoryBlock& destData)
{    
	auto state = parameters->copyState();
	//AudioParameterFloat* shift = (AudioParameterFloat*)parameters->getParameter("shift");
	//AudioParameterFloat* min = (AudioParameterFloat*)parameters.getParameter("shiftMinRange");
	//AudioParameterFloat* max = (AudioParameterFloat*)parameters.getParameter("shiftMaxRange");
	std::unique_ptr<XmlElement> xml(state.createXml());
	
	// encode range

	//XmlElement* shiftXmlElement = xml->getChildByAttribute("id", "shift");	
	//shiftXmlElement->setAttribute("minRange", min->get());
	//shiftXmlElement->setAttribute("maxRange", max->get());

	copyXmlToBinary(*xml, destData);
}

void SpectralAudioPlugin::setStateInformation (const void* data, int sizeInBytes)
{ 
	std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

	if ( xmlState.get() == nullptr ) { return; }
	if ( !xmlState->hasTagName(parameters->getState().getType()) ) { return;}
		
	parameters->replaceState(ValueTree::fromXml(*xmlState));	
	AudioProcessorEditor* editor =  this->getActiveEditor();
	if (editor != nullptr) {
		((SpectralAudioPluginUi*)editor)->onAudioValueTreeStateLoadedFromXmlState(parameters.get(), xmlState.get());		
	}
}

void SpectralAudioPlugin::setFftSize(int size) {	
    if (isInvalidFftModificationState()) { return; }
    
	m_audioProcessorInteractor->setFftSize(size);
	const int hopSize = m_audioProcessorInteractor->getHopSize();

	for(std::vector<float>& output : m_output)
	{
		output.resize(hopSize, 0.f);
	}

	for(std::vector<float>& input : m_input)
	{
		input.resize(hopSize, 0.f);
	}

	setLatencySamples(size + hopSize);
}

void SpectralAudioPlugin::checkForUpdates(VersionCheckThread::Listener* listener) {
	m_versionCheckThread.setListener(listener);
	m_versionCheckThread.startThread();
}

void SpectralAudioPlugin::initialiseParameters() {
    parameters = m_dependencyFactory->createParams(this);
    m_audioProcessorInteractor = m_dependencyFactory->createProcessor(this);
    
    auto fftSizesToRemove = m_dependencyFactory->fftSizesToNotInclude();
    m_fftSizeChoiceAdapter.remove(fftSizesToRemove);

    //m_audioProcessor->createParameters(parameters.get());
    parameters->createAndAddParameter(
                                      std::make_unique<AudioParameterChoice>(
                                                                             ParameterID("fft", 1),
                                                                             "FFT Size",
                                                                             m_fftSizeChoiceAdapter.fftStrings(),
                                                                             m_fftSizeChoiceAdapter.currentIndex()
                                                                             )
                                      );
    
    auto fftSizeChoices = (AudioParameterChoice*)parameters->getParameter("fft");
    m_fftSizeChoiceAdapter.listen(fftSizeChoices);
    
    parameters->createAndAddParameter(
                                      std::make_unique<AudioParameterChoice>(
                                                                             ParameterID("fftStyle", 1),
                                                                             "FFT style",
                                                                             m_fftStyleChoiceAdapter.fftStyleStrings(),
                                                                             m_fftStyleChoiceAdapter.currentIndex()
                                                                             )
                                      );
    
    auto fftStyleChoices = (AudioParameterChoice*)parameters->getParameter("fftStyle");
    m_fftStyleChoiceAdapter.listen(fftStyleChoices);
    
    
    parameters->createAndAddParameter(
                                      std::make_unique<AudioParameterChoice>(
                                                                             ParameterID("fftOverlaps", 1),
                                                                             "FFT Overlap count",
                                                                             m_fftOverlapsChoiceAdapter.overlapStrings(),
                                                                             m_fftOverlapsChoiceAdapter.currentIndex()
                                                                             )
                                      );
    
    auto fftOverlapChoices = (AudioParameterChoice*)parameters->getParameter("fftOverlaps");
    m_fftOverlapsChoiceAdapter.listen(fftOverlapChoices);
    
    parameters->createAndAddParameter(
                                      std::make_unique<AudioParameterChoice>(
                                                                             ParameterID("fftWindow", 1),
                                                                             "FFT Window type",
                                                                             m_fftWindowChoiceAdapter.fftWindowStrings(),
                                                                             m_fftWindowChoiceAdapter.currentIndex()
                                                                             )
                                      );
    
    auto fftWindowChoices = (AudioParameterChoice*)parameters->getParameter("fftWindow");
    m_fftWindowChoiceAdapter.listen(fftWindowChoices);
    

    auto valueTree = ValueTree(
        Identifier(
            this->getName().removeCharacters(" ")
        )
    );
    parameters->replaceState(valueTree);
    //m_ui = new SpectralAudioPluginUi(*this, parameters.get(), m_parameterUiComponent);
}
