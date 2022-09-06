#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../shared/PluginParameters.h"

//==============================================================================
class TiltSlider : public Component
{
public:
    TiltSlider(std::shared_ptr<PluginParameters> valueTreeState, Colour textColour, int textBoxHeight);
    ~TiltSlider();

    void paint (Graphics&) override;
    void resized() override;
private:
    typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

    Label tiltLabel;
    Slider tilt;
    std::unique_ptr<SliderAttachment> tiltAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TiltSlider)
};

