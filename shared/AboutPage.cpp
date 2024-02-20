
#include "AboutPage.h"


//==============================================================================
AboutPage::AboutPage(ResourceRepository& resources) :
    backButton("backButton", DrawableButton::ButtonStyle::ImageFitted),
    licenses(resources)
{
	
    backButton.setImages(resources.getNavigateBackIcon());
	backButton.onClick = [this]() {
		this->setVisible(false);
	};
        
    description.setText(JucePlugin_Desc, NotificationType::dontSendNotification);
    description.setFont(16.0);
	
	aboutMessageSection1.setJustificationType(Justification::topLeft);	
	aboutMessageSection2.setJustificationType(Justification::topLeft);
	
	downloadLink.setJustificationType(Justification::topLeft);	
	downloadLink.setColour(HyperlinkButton::ColourIds::textColourId, Colour(0xFF00BCD4));

	emailLink.setJustificationType(Justification::topLeft);
	emailLink.setColour(HyperlinkButton::ColourIds::textColourId, Colour(0xFF00BCD4));
    
    licenseButton.setButtonText("View licenses");
    licenseButton.onClick = [this] {
        licenses.setVisible(true);
    };

	addAndMakeVisible(&backButton);
    addAndMakeVisible(&description);
	addAndMakeVisible(&aboutMessageSection1);
	addAndMakeVisible(&downloadLink);
	addAndMakeVisible(&aboutMessageSection2);	
	addAndMakeVisible(&emailLink);
    addAndMakeVisible(&licenseButton);
    addChildComponent(&licenses);
    
    setVersionInfo(nullptr);
}

AboutPage::~AboutPage()
{
}

void AboutPage::paint (Graphics& g)
{    
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component			
}

void AboutPage::resized()
{
	static const int padding = 4;
	static const int componentSpacing = padding * 3;    
	
	backButton.setBounds(0, 0, 40, 40);
    auto y = backButton.getBottom() + componentSpacing;

	const Rectangle<int> localBounds = getBounds();
    description.setBounds(padding, y, getWidth() - padding, 60);
    
    y = description.getBottom() + componentSpacing;
    
	const Rectangle<int> aboutMessageSection1Bounds(localBounds.getX() + padding, y, getWidth(), 46);
	Rectangle<int> downloadLinkBounds = aboutMessageSection1Bounds;

	aboutMessageSection1.setBounds(aboutMessageSection1Bounds);
    
    y = aboutMessageSection1.getBottom() + componentSpacing;

	if (downloadLink.isVisible()) {		
		downloadLinkBounds = Rectangle<int>(localBounds.getX() + padding, y, getWidth(), 22);
		downloadLink.setBounds(downloadLinkBounds);
		downloadLink.changeWidthToFitText();		
        y = downloadLink.getBottom() + padding;
	}
	else {
		downloadLink.setBounds(0, 0, 0, 0);
	}

	Rectangle<int> aboutMessageSection2Bounds = downloadLinkBounds;
	if (aboutMessageSection2.isVisible()) {
		aboutMessageSection2Bounds = Rectangle<int>(localBounds.getX() + padding, y, getWidth(), getHeight() / 2);
		aboutMessageSection2.setBounds(aboutMessageSection2Bounds);
        y = aboutMessageSection2.getBottom() + padding;
	}
	else {
		aboutMessageSection2.setBounds(0, 0, 0, 0);
	}

	const Rectangle<int> emailBounds(localBounds.getX() + padding, y, getWidth(), 22);
	emailLink.setBounds(emailBounds);
    y = emailLink.getBottom() + padding;
    
    const Rectangle<int> licenseButtonBounds(localBounds.getX() + padding, y, getWidth() - padding, 22);
    licenseButton.setBounds(licenseButtonBounds);
    
    licenses.setBounds(localBounds);
}

void AboutPage::setVersionInfo(VersionInfo * versionInfo)
{			
	String section1String = "Version " + String(JucePlugin_VersionString);

#ifdef JUCE_DEBUG
    section1String << "\nBuild time: " << __DATE__  << __TIME__;
#endif
    
	String downloadString = "";
	String section2String = "";
	

	if (versionInfo != nullptr) {				
		section1String += "\n\nNew version available : " + versionInfo->getVersionName();				
		downloadString += versionInfo->getDownloadUrl().trimCharactersAtStart("https://www.");
		downloadLink.setURL(URL(versionInfo->getDownloadUrl()));
		
		section2String += "Release notes: \n";
		for(String s : versionInfo->getReleaseNotes()) {			
            section2String += "\n  > " + s;
		}
        
        section2String += "\n This plugin makes use of KissFFT developed by Mark Borgerding";
	}
	    
	aboutMessageSection1.setText(section1String, NotificationType::dontSendNotification);
    aboutMessageSection1.setColour(Label::textColourId, Colours::white);
	
	if (downloadString.isEmpty()) {
		downloadLink.setVisible(false);
	}
	else {
		downloadLink.setButtonText(downloadString);		
		downloadLink.setVisible(true);
	}

	if (section2String.isEmpty()) {
		aboutMessageSection2.setVisible(false);
	}
	else {
		aboutMessageSection2.setText(section2String, NotificationType::dontSendNotification);
        aboutMessageSection2.setColour(Label::textColourId, Colours::white);
		aboutMessageSection2.setVisible(true);
	}
	
	emailLink.setButtonText("Contact author");
	emailLink.setURL(URL("mailto:andrew.reeman@gmail.com"));
    this->resized();
	this->repaint();
}
