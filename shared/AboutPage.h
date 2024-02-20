#pragma once

#include "JuceHeader.h"
#include "VersionInfo.h"
#include "ResourceRepository.h"
#include "components/LicensesComponent.h"

//TODO: all components should be in shared/components folder

//==============================================================================
/*
*/
class AboutPage    : public Component
{
public:
    AboutPage(ResourceRepository& resources);
    ~AboutPage();

    void paint (Graphics&) override;
    void resized() override;
	void setVersionInfo(VersionInfo* info);

private:

	DrawableButton backButton;
    TextButton licenseButton;
    Label description;
	Label aboutMessageSection1;
	Label aboutMessageSection2;	
	HyperlinkButton downloadLink;
	HyperlinkButton emailLink;
    LicensesComponent licenses;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AboutPage)
};
