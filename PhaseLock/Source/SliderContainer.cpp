#include "SliderContainer.h"

SliderContainer::SliderContainer(std::shared_ptr<PhaseLockParameters> valueTreeState, Colour textColour, int) :	m_pluginParameters(valueTreeState)
{		
	m_lockPhase.setButtonText("Lock");
	m_lockPhase.setColour(ToggleButton::ColourIds::textColourId, textColour);
	m_lockPhase.setColour(ToggleButton::ColourIds::tickColourId, textColour);
	m_lockPhase.setColour(ToggleButton::ColourIds::tickDisabledColourId, textColour);
    m_lockPhase.setTooltip("Locks the frequency phases");
	m_phaseLockAttachment.reset(m_pluginParameters->createPhaseLockAttachment(m_lockPhase));
	addAndMakeVisible(&m_lockPhase);
    
    m_phaseMixLabel.setText("Phase", NotificationType::dontSendNotification);
    m_phaseMixLabel.setColour(Label::ColourIds::textColourId, textColour);
    addAndMakeVisible(&m_phaseMixLabel);
    
    m_phaseMixAttachment.reset(m_pluginParameters->createPhaseMixAttachment(m_phaseMixSlider));
    m_phaseMixSlider.setColour(Slider::ColourIds::textBoxTextColourId, textColour);
    m_phaseMixSlider.setTooltip("The mix between locked and unlocked phase");
    addAndMakeVisible(&m_phaseMixSlider);
    
    m_lockMag.setButtonText("Lock");
    m_lockMag.setColour(ToggleButton::ColourIds::textColourId, textColour);
    m_lockMag.setColour(ToggleButton::ColourIds::tickColourId, textColour);
    m_lockMag.setColour(ToggleButton::ColourIds::tickDisabledColourId, textColour);
    m_lockMag.setTooltip("Locks the frequency magnitudes");
    m_magLockAttachment.reset(m_pluginParameters->createMagLockAttachment(m_lockMag));
    addAndMakeVisible(&m_lockMag);
    
    m_magMixLabel.setText("Frequency", NotificationType::dontSendNotification);
    m_magMixLabel.setColour(Label::ColourIds::textColourId, textColour);
    addAndMakeVisible(m_magMixLabel);
    
    m_magMixAttachment.reset(m_pluginParameters->createMagMixAttachment(m_magMixSlider));
    m_magMixSlider.setColour(Slider::ColourIds::textBoxTextColourId, textColour);
    m_magMixSlider.setTooltip("The mix between locked and unlocked magnitudes");
    addAndMakeVisible(&m_magMixSlider);
        
    m_magTrackLabel.setText("Amplitude tracking", NotificationType::dontSendNotification);
    m_magTrackLabel.setColour(Label::ColourIds::textColourId, textColour);
    m_magTrackLabel.attachToComponent(&m_magTrackSlider, false);
    addAndMakeVisible(&m_magTrackLabel);
    
    m_magTrackAttachment.reset(m_pluginParameters->createMagTrackAttachment(m_magTrackSlider));
    m_magTrackSlider.setColour(Slider::ColourIds::textBoxTextColourId, textColour);
    m_magTrackSlider.setTooltip("The level of overall amplitude tracking");
    addAndMakeVisible(&m_magTrackSlider);
    
    m_randomPhaseLabel.setText("Random phase", NotificationType::dontSendNotification);
    m_randomPhaseLabel.setColour(Label::ColourIds::textColourId, textColour);
    m_randomPhaseLabel.attachToComponent(&m_randomPhaseSlider, false);
    addAndMakeVisible(&m_randomPhaseLabel);
    
    m_randomPhaseAttachment.reset(m_pluginParameters->createRandPhaseAttachment(m_randomPhaseSlider));
    m_randomPhaseSlider.setColour(Slider::ColourIds::textBoxTextColourId, textColour);
    m_randomPhaseSlider.setTooltip("The amount of random phase added");
    addAndMakeVisible(&m_randomPhaseSlider);
    
    m_startMorphPhaseAndMag.setColour(ToggleButton::ColourIds::textColourId, textColour);
	m_startMorphPhaseAndMag.setColour(ToggleButton::ColourIds::tickColourId, textColour);
	m_startMorphPhaseAndMag.setColour(ToggleButton::ColourIds::tickDisabledColourId, textColour);
    m_startMorphPhaseAndMag.setTooltip("Starts morphing phase and mag to a new locked target");
    m_morphMagAndPhaseAttachment.reset(m_pluginParameters->createMorphMagAndPhaseAttachment(m_startMorphPhaseAndMag));
    m_startMorphPhaseAndMag.setButtonText("Morph phase and frequency");
    addAndMakeVisible(&m_startMorphPhaseAndMag);
    
    m_morphDurationSecondsAttachment.reset(m_pluginParameters->createMorphDurationAttachment(m_morphDurationSeconds));
    m_morphDurationSeconds.setColour(Slider::ColourIds::textBoxTextColourId, textColour);
    m_morphDurationSeconds.setTooltip("The duration in seconds to morph from one target to the next");
    addAndMakeVisible(&m_morphDurationSeconds);
}

SliderContainer::~SliderContainer()
{
}

void SliderContainer::paint (Graphics& g)
{
}

void SliderContainer::resized()
{
	const int checkBoxHeight = 30;
    const int labelHeight = 30;
    const int sliderHeight = 50;

    const int xPadding = 4;    
    auto area = getLocalBounds().reduced(xPadding, 0).translated(xPadding, 0);
    auto originalArea = area;
    
    m_lockPhase.setBounds(area.removeFromTop(checkBoxHeight).removeFromRight(100));
    m_phaseMixLabel.setBounds(originalArea.removeFromTop(labelHeight).removeFromLeft(originalArea.getWidth() / 2));
    m_phaseMixSlider.setBounds(area.removeFromTop(sliderHeight));
    
    area.removeFromTop(16);
    originalArea = area;
    m_lockMag.setBounds(area.removeFromTop(checkBoxHeight).removeFromRight(100));
    m_magMixLabel.setBounds(originalArea.removeFromTop(labelHeight).removeFromLeft(originalArea.getWidth() / 2));
    m_magMixSlider.setBounds(area.removeFromTop(sliderHeight));
    
    area.removeFromTop(16);
    m_magTrackLabel.setBounds(area.removeFromTop(labelHeight));
    m_magTrackSlider.setBounds(area.removeFromTop(sliderHeight));
    
    area.removeFromTop(16);
    m_randomPhaseLabel.setBounds(area.removeFromTop(labelHeight));
    m_randomPhaseSlider.setBounds(area.removeFromTop(sliderHeight));
    
    area.removeFromTop(16);
    m_startMorphPhaseAndMag.setBounds(area.removeFromTop(checkBoxHeight));
    m_morphDurationSeconds.setBounds(area.removeFromTop(sliderHeight));
}

// TODO: remove this, there are no controlpoints in phase lock
void SliderContainer::controlPointsChanged(Array<float> outputValues, ControlPointComponent* component) {
    // get source points
    // set source points and output values on plugin parameter object
    m_pluginParameters->setControlPoints(outputValues, component->getSourcePoints());
}

void SliderContainer::onAudioValueTreeStateLoadedFromXmlState(PluginParameters* params, XmlElement* xml) {
//    auto controlPoints = pluginParameters->getControlPoints();
//    controlPoint.setSourcePoints(controlPoints);
}
