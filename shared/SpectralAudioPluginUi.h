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
#include "ResourceRepository.h"

// Themes
constexpr auto TEXT_COLOUR = "d7000000";
constexpr auto TEXT_SECONDARY_COLOUR = "8A000000";
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
    SpectralAudioPluginUi(SpectralAudioPlugin&, PluginParameters* pluginParameters, ParameterContainerComponent* parameterContainer);
    ~SpectralAudioPluginUi();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
	void handleCommandMessage(int) override;

	// VersionCheckThread::Listener methods
	void onNewVersionAvailable(VersionInfo* versionInfo) override;		
    
    // Inherited via AudioValueTreeStateOnLoadListener
	virtual void onAudioValueTreeStateLoadedFromXmlState(PluginParameters* newState, XmlElement* xmlState) override {
		parameterContainer->onAudioValueTreeStateLoadedFromXmlState(newState, xmlState);
        auto settingProperties = parameterContainer->getSettingsProperties();
        settingsPage.setProperties(settingProperties);
	}
    
    void onFftSizeChanged() {
        parameterContainer->onFftSizeChanged();
    }
    
    void onFftStyleChanged() {
        parameterContainer->onFftStyleChanged();
    }
//private:
	enum Messages {
		UPDATE_AVAILABLE,
        UPDATE_PROPERTIES
	};
	
	typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
	typedef AudioProcessorValueTreeState::ComboBoxAttachment ComboBoxAttachment;

	void aboutClicked();	

	void settingsClicked();

	const int textBoxHeight = 30;

	bool hasInit = false;
    
    Label title;
	const int parameterContainerHeight;
	ParameterContainerComponent* parameterContainer;
	
    Viewport parameterViewPort;
	Label fftComboLabel;
	ComboBox fftComboBox;
	std::unique_ptr<ComboBoxAttachment> fftComboBoxAttachment;	
	DrawableButton aboutButton;	
	DrawableButton settingsButton;
    ResourceRepository resourceRepository;
	SettingsPage settingsPage;	
	AboutPage aboutPage;

    std::unique_ptr<Drawable> infoIcon;
    std::unique_ptr<Drawable> settingsIcon;
	
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpectralAudioPluginUi)
};
