/*
  ==============================================================================

    ControlPoints.h
    Created: 16 Dec 2023 6:42:49pm
    Author:  Andrew Reeman

  ==============================================================================
*/

#pragma once

class ControlPoints {
public:
    void setPointsAndScale(Array<juce::Point<int>> newPoints, int newYScale)
    {
        this->points.swapWith(newPoints);
        this->yScale = newYScale;
    }
    
    void clear()
    {
        this->points.clear();
    }
    
    bool isEmpty() const {
        return this->points.isEmpty();
    }
    
    
    Array<juce::Point<int>> points;
    int yScale;
};
