#include "SpectralAudioPluginUi.h"

SpectralAudioPluginUi::SpectralAudioPluginUi(
    SpectralAudioPlugin& p,
    PluginParameters* valueTreeState,
    ParameterContainerComponent* _parameterContainer
) :
    AudioProcessorEditor(&p),
    parameterContainerHeight(_parameterContainer->getComponentHeight()),
    aboutButton("infoButton", DrawableButton::ButtonStyle::ImageFitted),
    settingsButton("settingsButton", DrawableButton::ButtonStyle::ImageFitted),
    settingsPage(resourceRepository),
    aboutPage(resourceRepository)
{
    this->parameterContainer = _parameterContainer;
	
	auto primaryTextColour = Colour::fromString(TEXT_COLOUR);
	title.setText(JucePlugin_Name, NotificationType::dontSendNotification);
	title.setColour(Label::ColourIds::textColourId, primaryTextColour);
	title.setFont(20.0);
	addAndMakeVisible(title);        

    parameterViewPort.setViewedComponent(this->parameterContainer);
    addAndMakeVisible(parameterViewPort);

	fftComboLabel.setText("FFT Size", NotificationType::dontSendNotification);
	fftComboLabel.setColour(Label::ColourIds::textColourId, primaryTextColour);
	addAndMakeVisible(&fftComboLabel);

	fftComboBox.setText("FFT Size");
	auto fftChoices = (AudioParameterChoice*)valueTreeState->getParameter("fft");
	
	int choiceIndex = 1;
	for (String fft : fftChoices->choices) {		
		fftComboBox.addItem(fft, choiceIndex);
		choiceIndex++;
	}
	
	fftComboBoxAttachment.reset( valueTreeState->createComboBoxAttachment("fft", fftComboBox) );//new ComboBoxAttachment(valueTreeState, "fft", fftComboBox));        
	addAndMakeVisible(fftComboBox);
	
	infoIcon = Drawable::createFromImageData(BinaryData::baselineinfo24px_svg, BinaryData::baselineinfo24px_svgSize);
	infoIcon->replaceColour(Colours::black, primaryTextColour);
			
    aboutButton.setImages(infoIcon.get());
	aboutButton.onClick = [this]() {
		this->aboutClicked();
	};

	addAndMakeVisible(&aboutButton);
    
    Array<PropertyComponent*> settings = this->parameterContainer->getSettingsProperties();
	if (!settings.isEmpty()) {
		settingsPage.setListener(this->parameterContainer);
		settingsPage.setProperties(settings);

		settingsIcon = Drawable::createFromImageData(BinaryData::baselinesettings20px_svg, BinaryData::baselinesettings20px_svgSize);
		settingsIcon->replaceColour(Colours::black, primaryTextColour);
        settingsButton.setImages(settingsIcon.get());
		settingsButton.onClick = [this]() {
			this->settingsClicked();
		};

		addAndMakeVisible(&settingsButton);		
		addChildComponent(&settingsPage);
        
	}

	addChildComponent(&aboutPage);
	setSize(300, 220 + parameterContainerHeight);
}

SpectralAudioPluginUi::~SpectralAudioPluginUi()
{
}

//==============================================================================
void SpectralAudioPluginUi::paint (Graphics& g)
{
	g.fillAll(Colours::white);
}

void SpectralAudioPluginUi::resized()
{
	if (!hasInit) {
		hasInit = true;
		((SpectralAudioPlugin*)&processor)->checkForUpdates(this);
        this->postCommandMessage(Messages::UPDATE_PROPERTIES);
	}
    
	auto xPadding = 8;
	auto yPadding = xPadding;

	auto titleHeight = 30;
	title.setBounds(xPadding, 0, getWidth() - xPadding, titleHeight);
        	
    auto y = title.getBottom() + yPadding;    
    parameterViewPort.setBounds(0, y, getWidth(), std::min(textBoxHeight + parameterContainerHeight, 800));
    parameterContainer->setBounds(xPadding, y, getWidth() - xPadding, textBoxHeight + parameterContainerHeight);
    
    y = parameterViewPort.getBottom() + yPadding;

	fftComboLabel.setBounds(xPadding, y, 100, textBoxHeight);

	y = fftComboLabel.getBottom() + 4;
	fftComboBox.setBounds(xPadding, y, 100, textBoxHeight);
	
	auto iconSize = titleHeight;	
	aboutButton.setBounds(getWidth() - iconSize, 0, iconSize, iconSize);	
	settingsButton.setBounds(aboutButton.getBounds().getX(), aboutButton.getBounds().getBottom(), iconSize, iconSize);

	aboutPage.setBounds(getBounds());
	settingsPage.setBounds(getBounds());
}

void SpectralAudioPluginUi::handleCommandMessage(int messageId)
{
	if (messageId == Messages::UPDATE_AVAILABLE) {
		aboutButton.getNormalImage()->replaceColour(Colour::fromString(TEXT_COLOUR), Colour::fromString(WARNING_COLOUR));				
		aboutButton.repaint();		
	}
    else if (messageId == Messages::UPDATE_PROPERTIES) {
        parameterContainer->onPropertiesChanged();
    }
}

void SpectralAudioPluginUi::onNewVersionAvailable(VersionInfo* newVersionInfo)
{
    aboutPage.setVersionInfo(newVersionInfo);
	this->postCommandMessage(Messages::UPDATE_AVAILABLE);			
}

void SpectralAudioPluginUi::aboutClicked()
{				
	aboutPage.setVisible(true);	
}

void SpectralAudioPluginUi::settingsClicked() {	
    // TODO: ensure we do not need to set the properties here, we should not need to
    //	const Array<PropertyComponent*> settings = this->parameterContainer->getSettingsProperties();
//	if (settings.isEmpty()) { return; }
    
//    settingsPage.setProperties(settings);
	settingsPage.setVisible(true);
}
