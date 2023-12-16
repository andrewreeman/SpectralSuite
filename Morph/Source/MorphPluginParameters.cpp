/*
  ==============================================================================
    MorphPluginParameters.cpp
  ==============================================================================
*/

#include "MorphPluginParameters.h"
#include "MorphInteractor.h"

MorphPluginParameters::MorphPluginParameters(AudioProcessor * processor) :
    PluginParameters(processor),    
    didSetInitialAudioState(false),
    controlPointComponent(nullptr)
{}


void MorphPluginParameters::controlPointsChanged(Array<float> controlPoints, ControlPointComponent* component) {
    
    auto newState = copyState();
    PluginParameters::replaceState(newState);
    
    lastPoints = component->getSourcePoints();
    if(listener != nullptr) {
        listener->controlPointsChanged(controlPoints);
    }
}

void MorphPluginParameters::replaceState(const ValueTree& newState) {
    PluginParameters::replaceState(newState);
    
    // TODO: the problem is that we are storing the UI values in the state
    // We should be storing the audio values instead and mapping this to the UI when required
    auto morphPoints = getMorphPointsFromState(newState);
    if(!morphPoints.isEmpty()) {
        controlPointComponent->setSourcePoints(morphPoints);
    }
    
    // TODO: this is an ugly hack because we are using replaceState internally thus we are using it in 2 places. We only want the below to run when called externaly
    if(!didSetInitialAudioState) {
        didSetInitialAudioState = true;
        if(listener != nullptr) {
            
        }
    }
}

ValueTree MorphPluginParameters::copyState() {
    ValueTree tree = PluginParameters::copyState();
    
    if(controlPointComponent == nullptr || !controlPointComponent->isShowing()) {
        return tree;
    }
        
    if (tree.getChildWithName("pointTree").isValid()) {
        tree.removeChild(tree.getChildWithName("pointTree"), nullptr);
    }
    
    ValueTree pointTree("pointTree");
    
    auto points = controlPointComponent->getSourcePoints();    
    if(points.isEmpty()) {
        return tree;
    }
    
    for(auto& point : points) {
        ValueTree valuePoint ("point");
        valuePoint.setProperty("x", point.x, nullptr);
        valuePoint.setProperty("y", point.y, nullptr);
        pointTree.appendChild(valuePoint, nullptr);
    }
    
    tree.addChild(pointTree, -1, nullptr);
    
    return tree;
}

void MorphPluginParameters::setControlPointComponent(ControlPointComponent* component) {
    component->setListener(this);
    this->controlPointComponent = component;
    auto morphPoints = getMorphPointsFromState(getState());
    
    if(!morphPoints.isEmpty()) {
        component->setSourcePoints(morphPoints);
    }
}

Array<juce::Point<int>> MorphPluginParameters::getMorphPointsFromState(ValueTree state) {
    if(controlPointComponent == nullptr || !state.getChildWithName("pointTree").isValid()) {
        return Array<juce::Point<int>>();
    }
    
    
    auto pointTree = state.getChildWithName("pointTree");
    
    Array<juce::Point<int>> points;
    for(int i=0; i<pointTree.getNumChildren(); ++i) {
        auto childPoint = pointTree.getChild(i);
        auto xValue = childPoint.getPropertyAsValue("x", nullptr);
        auto yValue = childPoint.getPropertyAsValue("y", nullptr);
        juce::Point<int> point;
        
        point.setX(xValue.getValue());
        point.setY(yValue.getValue());
        
        points.add(point);
    }
    
    return points;
}

void MorphPluginParameters::setAudioMorphPointsOnState(Array<float> audioMorphPoints, ValueTree state) {
    
}
