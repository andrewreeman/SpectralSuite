#pragma once

#include "JuceHeader.h"

//==============================================================================
/*
*/
class LicensesComponent : public Component
{
public:
    LicensesComponent();
    ~LicensesComponent();

    void paint (Graphics&) override;
    void resized() override;

private:
    void configureLicenseView();

    DrawableButton backButton;
    TextEditor licenses;
    Viewport licensesViewPort;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LicensesComponent)
};
