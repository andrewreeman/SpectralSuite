#pragma once

#include "../../shared/PluginParameters.h"
#include "../../shared/components/ControlPointComponent.h"

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
    Array<juce::Point<int>> getLastPoints() const { return lastPoints; }
    
    void controlPointsChanged (Array<float> controlPoints, ControlPointComponent* component) override;
private:
    Array<juce::Point<int>> getMorphPointsFromState(ValueTree state);
    
    Array<juce::Point<int>> lastPoints;
    Listener* listener;
    
    // TODO: this should really not be here. It is only here to query and set the parameters
    ControlPointComponent* controlPointComponent;
};
