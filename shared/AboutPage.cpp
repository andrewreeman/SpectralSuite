
#include "AboutPage.h"

//==============================================================================
AboutPage::AboutPage()
	: backButton("backButton", DrawableButton::ButtonStyle::ImageFitted)
{
	
	setVersionInfo(nullptr);
	ScopedPointer<Drawable> settingsIcon = Drawable::createFromImageData(BinaryData::baselinearrow_back24px_svg, BinaryData::baselinearrow_back24px_svgSize);
	settingsIcon->replaceColour(Colours::black, Colours::white);

	backButton.setImages(settingsIcon);
	backButton.onClick = [this]() {
		this->setVisible(false);
	};
	
	aboutMessageSection1.setJustificationType(Justification::topLeft);	
	aboutMessageSection2.setJustificationType(Justification::topLeft);
	
	downloadLink.setJustificationType(Justification::topLeft);	
	downloadLink.setColour(HyperlinkButton::ColourIds::textColourId, Colour(0xFF00BCD4));

	emailLink.setJustificationType(Justification::topLeft);
	emailLink.setColour(HyperlinkButton::ColourIds::textColourId, Colour(0xFF00BCD4));

	addAndMakeVisible(&backButton);
	addAndMakeVisible(&aboutMessageSection1);
	addAndMakeVisible(&downloadLink);
	addAndMakeVisible(&aboutMessageSection2);	
	addAndMakeVisible(&emailLink);	
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

	const Rectangle<int> localBounds = getBounds();
	const Rectangle<int> aboutMessageSection1Bounds(localBounds.getX() + padding, localBounds.getY() + padding + 40 + padding, localBounds.getWidth(), 46);
	Rectangle<int> downloadLinkBounds = aboutMessageSection1Bounds;

	aboutMessageSection1.setBounds(aboutMessageSection1Bounds);

	if (downloadLink.isVisible()) {		
		downloadLinkBounds = Rectangle<int>(localBounds.getX() + padding, aboutMessageSection1Bounds.getBottom() + componentSpacing, localBounds.getWidth(), 22);
		downloadLink.setBounds(downloadLinkBounds);
		downloadLink.changeWidthToFitText();		
	}
	else {
		downloadLink.setBounds(0, 0, 0, 0);
	}

	Rectangle<int> aboutMessageSection2Bounds = downloadLinkBounds;
	if (aboutMessageSection2.isVisible()) {
		aboutMessageSection2Bounds = Rectangle<int>(localBounds.getX() + padding, downloadLinkBounds.getBottom() + componentSpacing, localBounds.getWidth(), localBounds.getHeight() / 2);
		aboutMessageSection2.setBounds(aboutMessageSection2Bounds);
	}
	else {
		aboutMessageSection2.setBounds(0, 0, 0, 0);
	}

	const Rectangle<int> emailBounds(localBounds.getX() + padding, aboutMessageSection2Bounds.getBottom() + padding, localBounds.getWidth(), 22);
	emailLink.setBounds(emailBounds);
}

void AboutPage::setVersionInfo(VersionInfo * versionInfo)
{			
	String section1String = "Version " + String(JucePlugin_VersionString);			
	String downloadString = "";
	String section2String = "";
	

	if (versionInfo != nullptr) {				
		section1String += "\n\nNew version available : " + versionInfo->getVersionName();		
		downloadString += "Download from: ";
		downloadString += versionInfo->getDownloadUrl();
		downloadLink.setURL(URL(versionInfo->getDownloadUrl()));
		
		section2String += "Release notes: \n";
		for(String s : versionInfo->getReleaseNotes()) {			
			section2String += "\n  > " + s, Colours::white;
		}
	}
	
	aboutMessageSection1.setText(section1String, NotificationType::dontSendNotification);
	
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
		aboutMessageSection2.setVisible(true);
	}
	
	emailLink.setButtonText("Contact author");
	emailLink.setURL(URL("mailto:andrew.reeman@gmail.com"));
	
	this->repaint();
}
