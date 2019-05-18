#include "SpectralAudioPlugin.h"

// avoiding circular declaration
#include "SpectralAudioPluginUi.h"

const int SpectralAudioPlugin::FFT_OVERLAPS = 4;
const int SpectralAudioPlugin::INIT_FFT_INDEX = 4; // 2048 
const int SpectralAudioPlugin::N_CHANS = 2;

//==============================================================================
SpectralAudioPlugin::SpectralAudioPlugin(
	//std::unique_ptr<SpectralAudioProcessor> audioProcessor, std::unique_ptr<ParameterContainerComponentFactory> parameterComponentFactory
	DependencyCreator dependencyCreator,
	Array<int> fftSizesToRemove
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
	m_internalBufferReadWriteIndex(0),
	m_fftChoiceAdapter(INIT_FFT_INDEX),	
	//parameters(*this, nullptr),	
	m_fftSwitcher(this),
	m_versionCheckThread(VersionCode, "https://andrewreeman.github.io/spectral_suite_publish.json")
{			
	std::unique_ptr<Dependencies> dependencies = dependencyCreator(this);
	parameters = dependencies->getParams();
	m_parameterUiComponent = dependencies->moveUi();
	m_audioProcessor = dependencies->moveProcessor();

	//FileLogger* logger = new FileLogger(FileLogger::getSystemLogFileFolder().getChildFile("logs")
	//	.getChildFile("spectral_suite" + Time::getCurrentTime().formatted("%Y-%m-%d_%H-%M-%S"))
	//	.withFileExtension(".log")
	//	.getNonexistentSibling(),
	//	"Log started", 0);

	File logFile = FileLogger::getSystemLogFileFolder().getChildFile("SpectralSuite").getChildFile("spectral_suite.log");
	m_logger = std::unique_ptr<FileLogger>(
		new FileLogger(
		logFile,
			"Log started"
		)
	);
	Logger::setCurrentLogger(m_logger.get());
	
	m_fftChoiceAdapter.remove(fftSizesToRemove);

	//m_audioProcessor->createParameters(parameters.get());	
	parameters->createAndAddParameter(
		std::make_unique<AudioParameterChoice>(
			"fft", "FFT Size", m_fftChoiceAdapter.fftStrings(), m_fftChoiceAdapter.currentIndex()
		)
	);		
	
	auto fftChoices = (AudioParameterChoice*)parameters->getParameter("fft");	
	m_fftChoiceAdapter.listen(fftChoices);
	
	auto valueTree = ValueTree(
		Identifier( 
			this->getName().removeCharacters(" ") 
		)
	);
	parameters->replaceState(valueTree);		
	
	m_ui = new SpectralAudioPluginUi(*this, parameters.get(), m_parameterUiComponent);	
}

SpectralAudioPlugin::~SpectralAudioPlugin()
{
	Logger::setCurrentLogger(nullptr);
}

//==============================================================================
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
	int fftSize = m_fftChoiceAdapter.fftSize();	
	m_audioProcessor->prepareToPlay(fftSize, (int)sampleRate);	
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
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

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
	std::fill(m_output_L.begin(), m_output_L.end(), 0.f);
	std::fill(m_output_R.begin(), m_output_R.end(), 0.f);
}

void SpectralAudioPlugin::processBlock (AudioBuffer<float>& buffer, MidiBuffer&)
{					
	if (m_fftSwitcher.isThreadRunning()) {
		m_internalBufferReadWriteIndex = 0;
		return; 
	}

	if (m_fftChoiceAdapter.shouldChangeFft()) {
		m_fftSwitcher.startThread();
		return;
	}

	const int hopSize = m_audioProcessor->getHopSize();
	float** audio = buffer.getArrayOfWritePointers();				
	int numSamples = buffer.getNumSamples();
	int ioVSTBuffers = 0;

	while (numSamples--) {		
		if (m_internalBufferReadWriteIndex >= hopSize) {
			m_internalBufferReadWriteIndex = 0;
			emptyOutputs();
			m_audioProcessor->process(&m_Input_L[0], &m_output_L[0], &m_Input_R[0], &m_output_R[0]);
		}

		m_Input_L[m_internalBufferReadWriteIndex] = audio[0][ioVSTBuffers];
		m_Input_R[m_internalBufferReadWriteIndex] = audio[1][ioVSTBuffers];
		audio[0][ioVSTBuffers] = m_output_L[m_internalBufferReadWriteIndex];
		audio[1][ioVSTBuffers] = m_output_R[m_internalBufferReadWriteIndex];
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
	if (m_ui->getWidth() <= 0 || m_ui->getHeight() <= 0) {
		// recreate of ui after ui was closed
		m_ui = new SpectralAudioPluginUi(*this, parameters.get(), m_parameterUiComponent);
	}

	return m_ui;

	//if (!m_parameterUiComponent) { return nullptr; }
	
	
	//return editor;
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
	if ( !xmlState->hasTagName(parameters->getState().getType()) ) {	return;}
		
	parameters->replaceState(ValueTree::fromXml(*xmlState));			
	m_ui->onAudioValueTreeStateLoadedFromXmlState(parameters.get(), xmlState.get());		
}

void SpectralAudioPlugin::switchFftSize()
{
	setFftSize(m_fftChoiceAdapter.fftSize());
}

void SpectralAudioPlugin::setFftSize(int size) {	
	m_audioProcessor->setFftSize(size);
	int hopSize = m_audioProcessor->getHopSize();

	m_output_L.resize(hopSize, 0.f);
	m_output_R.resize(hopSize, 0.f);
	m_Input_L.resize(hopSize, 0.f);
	m_Input_R.resize(hopSize, 0.f);

	setLatencySamples(size);		
}

void SpectralAudioPlugin::checkForUpdates(VersionCheckThread::Listener* listener) {
	m_versionCheckThread.setListener(listener);
	m_versionCheckThread.startThread();
}
