/*
  ==============================================================================

    ControlPointComponent.cpp
    Created: 7 May 2020 1:11:36am
    Author:  Andrew Reeman

  ==============================================================================
*/


#include "ControlPointComponent.h"
#include "Spline.h"
#include "SplineHelper.h"

//==============================================================================
ControlPointComponent::ControlPointComponent() : draggedPoint(nullptr), draggedPointIndex(0), listener(nullptr), lastMouseClick(Time::getCurrentTime())
{
}

ControlPointComponent::~ControlPointComponent()
{
}

void ControlPointComponent::setInitialPoints(const Array<InitialPoint> initialPoints) {
    this->initialPoints = initialPoints;
}

void ControlPointComponent::mouseDown (const MouseEvent& e) {
        
    if (e.getNumberOfClicks() > 1 || e.mods.isAltDown()) {
        auto nearbyPoint = findNearbyPoint(e.getPosition(), nullptr);
        if(nearbyPoint == nullptr) { return; }
        
        for(auto initialPoint : initialPoints) {
            if( pointComparator.compareElements(initialPoint.referencePoint, *nearbyPoint) == 0 ) {
                if(initialPoint.isSticky()) {
                    return;
                }
            }
        }
        
        points.remove(nearbyPoint);
        repaint();
    }
    
    if(shouldDebounce(e)) { return; }

    if (points.size() == 0 || e.x > points.getLast().getX()) {
        points.add (e.getPosition());
        repaint();
    }
    else {
        auto nearbyPoint = findNearbyPoint(e.getPosition(), nullptr);
        if(nearbyPoint == nullptr) {
            points.add(e.getPosition());
                            
            points.sort(pointComparator);
            repaint();
        }
    }
}

void ControlPointComponent::mouseUp(const MouseEvent &event) {
    if(shouldDebounce(event)) { return; }
    lastMouseClick = event.eventTime;
    
    ignoreUnused(event);
    draggedPoint = nullptr;
    draggedPointIndex = 0;
        
    populateOutputValues();
}

void ControlPointComponent::mouseDrag (const MouseEvent& e)
{
    if(shouldDebounce(e)) { return; }
    
    juce::Point<int> mouseDownPosition = e.getPosition();
    if(draggedPoint != nullptr) {
        movePointWithMouse(mouseDownPosition);
    }
    else {
        auto nearestPoint = findNearbyPoint(mouseDownPosition, &draggedPointIndex);
        if(nearestPoint != nullptr) {
            draggedPoint = nearestPoint;
            movePointWithMouse(mouseDownPosition);
        }
    }
}

void ControlPointComponent::paint (Graphics& g)
{
//    g.fillAll(Colours::purple);
//    g.setColour(Colours::purple);
//
//    Rectangle<int> drawRect(getBounds().getX(), getBounds().getY(), getBounds().getWidth(), getBounds().getHeight());
//    g.drawRect(drawRect);
//

    if (points.size() >= 3)
    {
        Array<juce::Point<double>> dpoints;
        for (auto p : points)
            dpoints.add ({ double (p.getX()), double (p.getY())});
        
        Spline spline (dpoints);

        g.setColour (Colours::red);

        Path p;

        p.startNewSubPath (points.getFirst().toFloat());
        for (int x = points.getFirst().getX(); x < points.getLast().getX(); x++)
        {
            double y = spline.interpolate (x);
            if(y < 0.0) {
                y = 2.0;
            }
            else if(y > (double)getLocalBounds().getBottom()) {
                y = (double)getLocalBounds().getBottom() - 2.0;
            }
            
            p.lineTo (float (x), float (y));
        }
        p.lineTo (points.getLast().toFloat());

        g.strokePath (p, PathStrokeType (2));
    }
    else if(points.size() == 2) {
        g.setColour (Colours::red);

        Path p;

        p.startNewSubPath (points.getFirst().toFloat());
        for (auto& point : points)
        {
            p.lineTo (float (point.x), float (point.y));
        }

        g.strokePath (p, PathStrokeType (2));
    }

    g.setColour (Colours::blue);
    for (auto pt : points)
        g.fillEllipse (pt.getX() - 4.0f, pt.getY() - 4.0f, 8.0f, 8.0f);

    if (points.isEmpty())
        g.drawText ("Click from left to right to add points.\n Double click to remove.", getLocalBounds(), Justification::centred);
}

void ControlPointComponent::resized() {
    if(initialPoints.isEmpty()) {
        populateOutputValues();
        return;        
    }
    
    if(!points.isEmpty()) {
        populateOutputValues();
        return;
    }
    
    Rectangle<int> bounds = getLocalBounds();
    int h = bounds.getHeight();
    int w = bounds.getWidth();
    int boundsX = bounds.getX();
    int boundsY = bounds.getY();
    
    for(auto& initialPoint : initialPoints) {
        int x = (int)(initialPoint.x * (double)w) + boundsX;
        int y = (int)(initialPoint.y * (double)h) + boundsY;
        juce::Point<int> p(x, y);
        points.add(p);        
        initialPoint.referencePoint.setX(x);
        initialPoint.referencePoint.setY(y);
    }
    
    points.sort(pointComparator);
    populateOutputValues();
}

void ControlPointComponent::movePointWithMouse(const juce::Point<int> &mouseDownPosition) {
    int newX = clipX(mouseDownPosition.getX(), draggedPointIndex);
    int newY = clipY(mouseDownPosition.getY(), draggedPointIndex);
    draggedPoint->setXY(newX, newY);
    this->repaint();
}

juce::Point<int>* ControlPointComponent::findNearbyPoint(juce::Point<int> pointToCompare, int* out_nearestPointIndex = nullptr) {
    for(int i=0; i<points.size(); ++i) {
        auto p = &points.getReference(i);
                        
        if(POINTS_ARE_CLOSE(p, pointToCompare)) {
            if(out_nearestPointIndex != nullptr) {
                *out_nearestPointIndex = i;
            }
            
            return p;
        }
    }
    
    return nullptr;
}

int ControlPointComponent::clipX(int newX, int pointIndex) {
    if(pointIndex == 0) {
        return points.getFirst().getX();
    }
    
    if(pointIndex == points.size() -1) {
        return points.getLast().getX();
    }
    
    auto pointBefore = points.getReference(pointIndex - 1);
    auto pointAfter = points.getReference(pointIndex + 1);
    
    static int threshold = 5;
    if(newX < (pointBefore.getX() + threshold)) {
        return points.getReference(pointIndex).getX();
    }
    
    if(newX > (pointAfter.getX() - threshold)) {
        return points.getReference(pointIndex).getX();
    }
    
    return newX;
}

int ControlPointComponent::clipY(int newY, int pointIndex) {
    auto padding = 0;
    auto bounds = getBounds();
    auto bottom = bounds.getBottom() - padding;
    auto top = bounds.getY() + padding;
    
    if(newY > bottom) {
        return bottom;
    }
    
    if(newY < top) {
        return top;
    }
    
    return newY;
}

void ControlPointComponent::populateOutputValues() {
    double bottom = (double)getLocalBounds().getBottom();
    auto output = SplineHelper::getAudioSplineValues(points, bottom);
    outputValues.clearQuick();
    outputValues.addArray(output);
    notifyChanged();
}

void ControlPointComponent::setSourcePoints(Array<juce::Point<int>> points) {
    this->points = points;
    repaint();
    
    populateOutputValues();    
}


bool ControlPointComponent::shouldDebounce(const MouseEvent& e) {
    return (e.eventTime.toMilliseconds() - lastMouseClick.toMilliseconds()) < 300;
}
