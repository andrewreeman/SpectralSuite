/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SpectralAudioPlugin.h"
#include "VersionCheck.h"
#include "ParameterComponentFactory.h"
#include "SettingsPage.h"

// Themes
constexpr auto TEXT_COLOUR = "d7000000";
constexpr auto WARNING_COLOUR = "FFF44336";

//==============================================================================
/**
*/

class SpectralAudioProcessorEditor :
	public AudioProcessorEditor, 
	public VersionCheckThread::Listener	
{
public:	    
	SpectralAudioProcessorEditor(SpectralAudioPlugin&, AudioProcessorValueTreeState& valueTreeState, ParameterComponentFactory& parameterContainer);
    ~SpectralAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
	void handleCommandMessage(int) override;

	// VersionCheckThread::Listener methods
	void onNewVersionAvailable(std::unique_ptr<VersionInfo> versionInfo);		
	
private:
	enum Messages {
		UPDATE_AVAILABLE
	};
	
	typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
	typedef AudioProcessorValueTreeState::ComboBoxAttachment ComboBoxAttachment;

	void aboutClicked();	

	void settingsClicked();

	const int textBoxHeight = 30;
	const AudioProcessorValueTreeState& valueTreeState;

	bool hasInit = false;
	Label title;	

	const int parameterContainerHeight;
	std::unique_ptr<Component> parameterContainer;	
	
	Label fftComboLabel;
	ComboBox fftComboBox;
	std::unique_ptr<ComboBoxAttachment> fftComboBoxAttachment;	
	DrawableButton aboutButton;	
	DrawableButton settingsButton;
	SettingsPage settingsPage;	

	std::unique_ptr<VersionInfo> versionInfo;
	
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpectralAudioProcessorEditor)
};
