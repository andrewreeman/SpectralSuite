#include "../JuceLibraryCode/JuceHeader.h"
#include "MorphSlider.h"

//==============================================================================
MorphSlider::MorphSlider(std::shared_ptr<MorphPluginParameters> valueTreeState, Colour textColour, int textBoxHeight)
{
    this->addAndMakeVisible(controlPointComponent);
	this->valueTreeState = valueTreeState;
    this->valueTreeState.get()->setControlPointComponent(&this->controlPointComponent);
}

MorphSlider::~MorphSlider()
{
}

void MorphSlider::paint(Graphics& g)
{
	g.fillAll(Colours::white); 
}

void MorphSlider::resized()
{

    InitialPoint p1(0.0, 1.0);
    p1.lockX = true;
    p1.lockY = true;
    
    InitialPoint p2(1.0, 0.0);
    p2.lockX = true;
    p2.lockY = true;
    
    Array<InitialPoint> initialPoints(p1, p2);
        
    controlPointComponent.setInitialPoints(initialPoints);
    auto lastPoints = this->valueTreeState->getLastPoints();
    if(!lastPoints.isEmpty()) {
        controlPointComponent.setSourcePoints(lastPoints);
    }
    
    controlPointComponent.setBounds(0, 0, getWidth() * 0.8, getHeight());
}
