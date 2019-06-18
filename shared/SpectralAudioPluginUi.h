#pragma once

#include "JuceHeader.h"
#include "SpectralAudioPlugin.h"
#include "AudioValueTreeOnLoadListener.h"
#include "VersionCheck.h"
#include "ParameterContainerComponentFactory.h"
#include "SettingsPage.h"
#include "ParameterContainerComponent.h"
#include "PluginParameters.h"
#include "AboutPage.h"

// Themes
constexpr auto TEXT_COLOUR = "d7000000";
constexpr auto WARNING_COLOUR = "FFF44336";

//==============================================================================
/**
*/

class SpectralAudioPluginUi :
	public AudioProcessorEditor, 
	public VersionCheckThread::Listener	,
	public AudioValueTreeStateOnLoadListener
{
public:	    
	SpectralAudioPluginUi(SpectralAudioPlugin&, PluginParameters* pluginParameters, std::shared_ptr<ParameterContainerComponent> parameterContainer);
    ~SpectralAudioPluginUi();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
	void handleCommandMessage(int) override;

	// VersionCheckThread::Listener methods
	void onNewVersionAvailable(std::unique_ptr<VersionInfo> versionInfo);		

	// Inherited via AudioValueTreeStateOnLoadListener
	virtual void onAudioValueTreeStateLoadedFromXmlState(PluginParameters* newState, XmlElement* xmlState) override {
		parameterContainer->onAudioValueTreeStateLoadedFromXmlState(newState, xmlState);
	}
	
private:
	enum Messages {
		UPDATE_AVAILABLE
	};
	
	typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
	typedef AudioProcessorValueTreeState::ComboBoxAttachment ComboBoxAttachment;

	void aboutClicked();	

	void settingsClicked();

	const int textBoxHeight = 30;
	const PluginParameters* valueTreeState;

	bool hasInit = false;
	Label title;	

	const int parameterContainerHeight;
	std::shared_ptr<ParameterContainerComponent> parameterContainer;	
	
	Label fftComboLabel;
	ComboBox fftComboBox;
	std::unique_ptr<ComboBoxAttachment> fftComboBoxAttachment;	
	DrawableButton aboutButton;	
	DrawableButton settingsButton;
	SettingsPage settingsPage;	
	AboutPage aboutPage;

	//std::unique_ptr<VersionInfo> versionInfo;
	
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpectralAudioPluginUi)	
};
