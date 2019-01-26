#include "SpectralAudioPlugin.h"
#include "../../shared/PluginEditor.h"

const int SpectralAudioPlugin::FFT_OVERLAPS = 4;
const int SpectralAudioPlugin::INIT_FFT_INDEX = 4; // 2048 
const int SpectralAudioPlugin::N_CHANS = 2;

//==============================================================================
SpectralAudioPlugin::SpectralAudioPlugin(
	std::unique_ptr<SpectralAudioProcessor> audioProcessor, std::unique_ptr<ParameterComponentFactory> parameterComponentFactory, Array<int> fftSizesToRemove
)
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),

#endif		
	m_internalBufferReadWriteIndex(0),	
	m_fftChoiceAdapter(INIT_FFT_INDEX),
	m_audioProcessor(audioProcessor.release()),
	m_parameterUiComponentFactory(parameterComponentFactory.release()),
	parameters(*this, nullptr),
	m_fftSwitcher(this),
	m_versionCheckThread(JucePlugin_VersionCode, "https://andrewreeman.github.io/spectral_suite_publish.json")
{			
	m_fftChoiceAdapter.remove(fftSizesToRemove);

	m_audioProcessor->createParameters(&parameters);	
	parameters.createAndAddParameter(
		std::make_unique<AudioParameterChoice>(
			"fft", "FFT Size", m_fftChoiceAdapter.fftStrings(), m_fftChoiceAdapter.currentIndex()
		)
	);		
	
	auto fftChoices = (AudioParameterChoice*)parameters.getParameter("fft");
	m_fftChoiceAdapter.listen(fftChoices);
	
	auto valueTree = ValueTree(
		Identifier( 
			this->getName().removeCharacters(" ") 
		)
	);
	parameters.replaceState(valueTree);			
}

SpectralAudioPlugin::~SpectralAudioPlugin()
{
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

void SpectralAudioPlugin::setCurrentProgram (int index)
{
}

const String SpectralAudioPlugin::getProgramName (int index)
{
    return {};
}

void SpectralAudioPlugin::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void SpectralAudioPlugin::prepareToPlay (double sampleRate, int samplesPerBlock)
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

void SpectralAudioPlugin::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{			
	if (m_fftSwitcher.isThreadRunning()) {
		m_internalBufferReadWriteIndex = 0;
		return; 
	}

	if (m_fftChoiceAdapter.shouldChangeFft()) {
		m_fftSwitcher.startThread();
		return;
	}

	float** audio = buffer.getArrayOfWritePointers();
	
	int numSamples = buffer.getNumSamples();	
	
	int hopSize = m_audioProcessor->getHopSize();
	int blockSize = numSamples >= hopSize ? hopSize : numSamples;		
	int ioVSTBuffers = 0;
	while (numSamples--) {				
		m_Input_L[m_internalBufferReadWriteIndex] = audio[0][ioVSTBuffers];
		m_Input_R[m_internalBufferReadWriteIndex] = audio[1][ioVSTBuffers];
		audio[0][ioVSTBuffers] = m_output_L[m_internalBufferReadWriteIndex];
		audio[1][ioVSTBuffers] = m_output_R[m_internalBufferReadWriteIndex];
		ioVSTBuffers++;
		m_internalBufferReadWriteIndex++;

		if (m_internalBufferReadWriteIndex >= hopSize) {			
			m_internalBufferReadWriteIndex = 0;
			emptyOutputs();						
			m_audioProcessor->process(&m_Input_L[0], &m_output_L[0], &m_Input_R[0], &m_output_R[0]);			
		}
	}			
}

//==============================================================================
bool SpectralAudioPlugin::hasEditor() const
{
    return true;
}

AudioProcessorEditor* SpectralAudioPlugin::createEditor()
{
    //return new SpectralGateAudioProcessorEditor (*this, parameters, new FrequencySlider(parameters, Colour::fromString(TEXT_COLOUR), 30));	
	return new SpectralGateAudioProcessorEditor(*this, parameters, *m_parameterUiComponentFactory);
}

//==============================================================================
void SpectralAudioPlugin::getStateInformation (MemoryBlock& destData)
{    
	auto state = parameters.copyState();
	std::unique_ptr<XmlElement> xml(state.createXml());
	copyXmlToBinary(*xml, destData);

}

void SpectralAudioPlugin::setStateInformation (const void* data, int sizeInBytes)
{ 
	std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

	if (xmlState.get() != nullptr) {
		if (xmlState->hasTagName(parameters.state.getType())) {
			parameters.replaceState(ValueTree::fromXml(*xmlState));
		}
	}
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
