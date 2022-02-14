#pragma once

#include "JuceHeader.h"
#include "VersionInfo.h"

//==============================================================================
/*
*/
class AboutPage    : public Component
{
public:
    AboutPage();
    ~AboutPage();

    void paint (Graphics&) override;
    void resized() override;
	void setVersionInfo(VersionInfo* info);

private:

	DrawableButton backButton;
    Label description;
	Label aboutMessageSection1;
	Label aboutMessageSection2;	
	HyperlinkButton downloadLink;
	HyperlinkButton emailLink;
	
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AboutPage)
};
