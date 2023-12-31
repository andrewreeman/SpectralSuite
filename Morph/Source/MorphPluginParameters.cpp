/*
  ==============================================================================
    MorphPluginParameters.cpp
  ==============================================================================
*/

#include "MorphPluginParameters.h"
#include "MorphInteractor.h"
#include "../../shared/components/SplineHelper.h"

MorphPluginParameters::MorphPluginParameters(AudioProcessor * processor) :
    PluginParameters(processor),    
    didSetInitialAudioState(false),
    controlPointComponent(nullptr)
{}

void MorphPluginParameters::controlPointsChanged(Array<float> controlPoints, ControlPointComponent* component) {
    
    lastPoints = component->getSourcePoints();
    this->lastPointsV2.setPointsAndScale(component->getSourcePoints(), component->getBounds().getBottom());
    
    auto newState = copyState();
    PluginParameters::replaceState(newState);
    
    if(listener != nullptr) {
        listener->controlPointsChanged(controlPoints);
    }
}

void MorphPluginParameters::replaceState(const ValueTree& newState) {
    PluginParameters::replaceState(newState);
    
    bool paramsEmptyBeforePopulate = this->lastPointsV2.isEmpty();
    
    setPointsFromState(newState, this->lastPointsV2);
    if(!lastPointsV2.points.isEmpty() && controlPointComponent != nullptr) {
        controlPointComponent->setSourcePoints(lastPointsV2.points);
    }
    
    // TODO: this is an ugly hack because we are using replaceState internally thus we are using it in 2 places. We only want the below to run when called externaly
    bool paramsDidChange = paramsEmptyBeforePopulate && !this->lastPointsV2.isEmpty();
    if(!didSetInitialAudioState || paramsDidChange) {
        didSetInitialAudioState = true;
        this->triggerControlPointsChanged();
    }
}

void MorphPluginParameters::triggerControlPointsChanged() {
    if(listener != nullptr && !lastPointsV2.points.isEmpty()) {
        auto audioValues = SplineHelper::getAudioSplineValues(lastPointsV2.points, lastPointsV2.yScale);
        listener->controlPointsChanged(audioValues);
    };
}

ValueTree MorphPluginParameters::copyState() {
    ValueTree tree = PluginParameters::copyState();
    
    if (tree.getChildWithName("pointTree").isValid()) {
        tree.removeChild(tree.getChildWithName("pointTree"), nullptr);
    }
    
    ValueTree pointTree("pointTree");
    
    auto points = lastPointsV2.points;
    if(points.isEmpty()) {
        return tree;
    }
    
    pointTree.setProperty("ylimit", lastPointsV2.yScale, nullptr);
    
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
    this->setPointsFromState(getState(), this->lastPointsV2);
    
    if(!lastPointsV2.points.isEmpty()) {
        component->setSourcePoints(lastPointsV2.points);
    }
}

void MorphPluginParameters::setPointsFromState(ValueTree state, ControlPoints &controlPoints) {
    auto xmlStr = state.toXmlString();
    if(!state.getChildWithName("pointTree").isValid()) {
        controlPoints.clear();
        return;
    }
    
    auto pointTree = state.getChildWithName("pointTree");
    
    int yLimit;
    if (pointTree.hasProperty("ylimit"))
    {
        yLimit = pointTree.getPropertyAsValue("ylimit", nullptr).getValue();
    }
    else if(controlPointComponent != nullptr) {
        yLimit = controlPointComponent->getLocalBounds().getBottom();
    }
    else {
        controlPoints.clear();
        return;
    }
    
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
    
    controlPoints.setPointsAndScale(points, yLimit);
}

void MorphPluginParameters::setAudioMorphPointsOnState(Array<float> audioMorphPoints, ValueTree state) {
    
}
