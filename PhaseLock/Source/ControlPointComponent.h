/*
  ==============================================================================

    ControlPointComponent.h
    Created: 7 May 2020 1:11:36am
    Author:  Andrew Reeman

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#define POINTS_ARE_CLOSE(p1,p2)  p1->getDistanceFrom(p2) < 20

//==============================================================================
/*
*/
struct PointXAxisComparator {
    static int compareElements (juce::Point<int>& first, juce::Point<int>& second) {
        return DefaultElementComparator<int>().compareElements(first.x, second.x);
    }
};

/// x and y in range of 0-1
/// lockX and lockY will mean that if these points will not move in the locked axis
struct InitialPoint {
    double x;
    double y;
    bool lockX;
    bool lockY;
    
    juce::Point<int> referencePoint;
    
    InitialPoint(double initialX, double initialY) : x(initialX), y(initialY) {}
    
    bool isSticky() {
        return lockX || lockY;
    }    
};

class ControlPointComponent : public Component
{
    
public:
    class Listener
    {
    public:       
       virtual ~Listener() = default;
       virtual void controlPointsChanged (Array<float> controlPoints, ControlPointComponent* component) = 0;
    };
    
    ControlPointComponent();
    ~ControlPointComponent();

    void paint (Graphics&) override;
    void resized() override;
    
    void mouseDown (const MouseEvent& e) override;
    void mouseUp(const MouseEvent &event) override;
    void mouseDrag (const MouseEvent& e) override;
    
    void setInitialPoints(const Array<InitialPoint> initialPoints);
    void setListener(Listener* l) { listener = l; }
    Array<juce::Point<int>> getSourcePoints() { return points; }
    void setSourcePoints(Array<juce::Point<int>> points);

private:
    int clipX(int newX, int pointIndex);
    juce::Point<int>* findNearbyPoint(juce::Point<int> pointToCompare, int* out_nearestPointIndex);
    void movePointWithMouse(const juce::Point<int> &mouseDownPosition);
    void populateOutputValues();

    Array<juce::Point<int>> points;
    Array<InitialPoint> initialPoints;
    
    juce::Point<int>* draggedPoint;
    Array<float> outputValues;
    int draggedPointIndex;
    PointXAxisComparator pointComparator;
    Listener* listener;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ControlPointComponent)
};
