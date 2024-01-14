#pragma once

#include "../../shared/PluginParameters.h"
#include "../../shared/components/ControlPointComponent.h"
#include "ControlPoints.h"

class MorphInteractor;

class MorphPluginParameters : public PluginParameters, public ControlPointComponent::Listener {
public:
    class Listener {
    public:
        virtual ~Listener() = default;
        virtual void controlPointsChanged(Array<float> controlPoints) = 0;
    };
        
	MorphPluginParameters(AudioProcessor* processor);
    
    void replaceState(const ValueTree& newState) override;
    ValueTree copyState() override;    	
		
    void setListener(Listener* listener){this->listener = listener;}
    void setControlPointComponent(ControlPointComponent* component);
    void triggerControlPointsChanged();
    Array<juce::Point<int>> getLastPoints() const { return lastPoints; }
    
    void controlPointsChanged (Array<float> controlPoints, ControlPointComponent* component) override;
private:
    void setPointsFromState(ValueTree state, ControlPoints &controlPoints);
    void setAudioMorphPointsOnState(Array<float> audioMorphPoints, ValueTree state);
    
    // TODO: this is an awfully named variable
    Array<juce::Point<int>> lastPoints;
    
    // TODO: use a unique pointer instead?
    ControlPoints lastPointsV2;
    
    
    Listener* listener;
    bool didSetInitialAudioState;
    
    // TODO: this should really not be here. It is only here to query and set the parameters
    ControlPointComponent* controlPointComponent;
};
