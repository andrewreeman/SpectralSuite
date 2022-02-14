#include "SettingsPage.h"

//==============================================================================
SettingsPage::SettingsPage() :	
	backButton("backButton", DrawableButton::ButtonStyle::ImageFitted),
	listener(nullptr)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.	
	addAndMakeVisible(&propertyPanel);

	std::unique_ptr<Drawable> settingsIcon = Drawable::createFromImageData(BinaryData::baselinearrow_back24px_svg, BinaryData::baselinearrow_back24px_svgSize);
	settingsIcon->replaceColour(Colours::black, Colours::white);

    backButton.setImages(settingsIcon.release());
	backButton.onClick = [this]() {
		this->backButtonClicked();
	};

	addAndMakeVisible(backButton);
}

SettingsPage::~SettingsPage()
{
}

void SettingsPage::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::white);
    g.setFont (14.0f);
        
    g.drawText ("Settings", getLocalBounds() + juce::Point<int>(0, 10),
                Justification::centredTop, true);   // draw some placeholder text
	
}



void SettingsPage::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
	
	backButton.setBounds(0, 0, 40, 40);

	propertyPanel.setBounds(
		getBounds()
		.translated(0, 40)
		.expanded(0, -40)
	);
}

//
//Array<PropertyComponent*> SettingsPage::createSliders(int howMany)
//{
//	Array<PropertyComponent*> comps;
//
//	for (int i = 0; i < howMany; ++i)
//		comps.add(new SliderPropertyComponent(Value(Random::getSystemRandom().nextDouble()), "slider " + String(i), 0.0, 100.0, 0.001));
//
//
//	return comps;
//}

void SettingsPage::backButtonClicked()
{
	this->setVisible(false);
	if (this->listener != nullptr) {
		this->listener->onPropertiesChanged();
	}

}
