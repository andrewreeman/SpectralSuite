#include "SpectralAudioPluginUi.h"

SpectralAudioPluginUi::SpectralAudioPluginUi(SpectralAudioPlugin& p, PluginParameters* valueTreeState, std::shared_ptr<ParameterContainerComponent> _parameterContainer)
	: AudioProcessorEditor(&p), 
	valueTreeState(valueTreeState),
    parameterContainerHeight(_parameterContainer->getComponentHeight()),
    aboutButton("infoButton", DrawableButton::ButtonStyle::ImageFitted),
    settingsButton("settingsButton", DrawableButton::ButtonStyle::ImageFitted)
{				
	this->parameterContainer = _parameterContainer;
	
	auto textColour = Colour::fromString(TEXT_COLOUR);
		
	title.setText(JucePlugin_Name, NotificationType::dontSendNotification);
	title.setColour(Label::ColourIds::textColourId, textColour);
	title.setFont(20.0);
	addAndMakeVisible(title);

	addAndMakeVisible(parameterContainer.get());
	fftComboLabel.setText("FFT Size", NotificationType::dontSendNotification);
	fftComboLabel.setColour(Label::ColourIds::textColourId, textColour);
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
	
	ScopedPointer<Drawable> infoIcon = Drawable::createFromImageData(BinaryData::baselineinfo24px_svg, BinaryData::baselineinfo24px_svgSize);
	infoIcon->replaceColour(Colours::black, textColour);
			
	aboutButton.setImages(infoIcon);
	aboutButton.onClick = [this]() {
		this->aboutClicked();
	};

	addAndMakeVisible(&aboutButton);
	
	Array<PropertyComponent*> settings = this->parameterContainer.get()->getSettingsProperties();	
	if (!settings.isEmpty()) {
		settingsPage.setListener(this->parameterContainer.get());
		settingsPage.setProperties(settings);

		ScopedPointer<Drawable> settingsIcon = Drawable::createFromImageData(BinaryData::baselinesettings20px_svg, BinaryData::baselinesettings20px_svgSize);
		settingsIcon->replaceColour(Colours::black, textColour);
		settingsButton.setImages(settingsIcon);
		settingsButton.onClick = [this]() {
			this->settingsClicked();
		};

		addAndMakeVisible(&settingsButton);		
		addChildComponent(&settingsPage);
	}	

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
	}
    
	auto xPadding = 8;
	auto yPadding = xPadding * 4;

	auto titleHeight = 30;
	title.setBounds(xPadding, 0, getWidth() - xPadding, titleHeight);
	
	auto y = title.getBottom() + yPadding;			
	parameterContainer->setBounds(xPadding, y, getWidth() - xPadding, textBoxHeight + parameterContainerHeight);
	y = parameterContainer->getBottom() + yPadding;

	fftComboLabel.setBounds(xPadding, y, 100, textBoxHeight);

	y = fftComboLabel.getBottom() + 4;
	fftComboBox.setBounds(xPadding, y, 100, textBoxHeight);
	
	auto iconSize = titleHeight;	
	aboutButton.setBounds(getWidth() - iconSize, 0, iconSize, iconSize);	
	settingsButton.setBounds(aboutButton.getBounds().getX(), aboutButton.getBounds().getBottom(), iconSize, iconSize);
	
	settingsPage.setBounds(getBounds());	
}

void SpectralAudioPluginUi::handleCommandMessage(int messageId)
{
	if (messageId == Messages::UPDATE_AVAILABLE) {
		aboutButton.getNormalImage()->replaceColour(Colour::fromString(TEXT_COLOUR), Colour::fromString(WARNING_COLOUR));				
		aboutButton.repaint();		
	}
}

void SpectralAudioPluginUi::onNewVersionAvailable(std::unique_ptr<VersionInfo> newVersionInfo)
{	
	this->versionInfo.reset(newVersionInfo.release());
	this->postCommandMessage(Messages::UPDATE_AVAILABLE);			
}

void SpectralAudioPluginUi::aboutClicked()
{		
	String aboutMessage = "Version " + String(JucePlugin_VersionString);	
	if (versionInfo != nullptr) {
		aboutMessage += "\n\n New version available: " + versionInfo->getVersionName();
		aboutMessage += "\n\n Download from: " + versionInfo->getDownloadUrl();
		aboutMessage += "\n\n Release notes: \n\n";
		for(String s : versionInfo->getReleaseNotes()) {
			aboutMessage += "\n> " + s;
		}
	}	

	aboutMessage += "\n\nMessage andrew.reeman@gmail.com for support.";


	AlertWindow::AlertIconType icon = AlertWindow::NoIcon;
	AlertWindow::showMessageBoxAsync(
		icon, TRANS("About"),
		aboutMessage,
		"OK"
	);
}

void SpectralAudioPluginUi::settingsClicked() {	
	const Array<PropertyComponent*> settings = this->parameterContainer.get()->getSettingsProperties();
	if (settings.isEmpty()) { return; }
	
	
	settingsPage.setProperties(settings);		
	settingsPage.setVisible(true);
	

	/*AlertWindow::AlertIconType icon = AlertWindow::NoIcon;
	AlertWindow::showMessageBoxAsync(
		icon, TRANS("Settings"),
		"Settings",
		"OK"
	);*/
}
