/*
 ==============================================================================
 
 SplineHelper.cpp
 Created: 28 Aug 2023 6:02:16pm
 Author:  Andrew Reeman
 
 ==============================================================================
 */

#include "SplineHelper.h"
#include "Spline.h"

namespace SplineHelper {
Array<float> getAudioSplineValues(Array<juce::Point<int>> points, float yLimit) {
    Array<float> outputValues;
    
    if(points.size() < 2) { return outputValues; }
    
    const int resolution = 128;
    auto pStart = points.getFirst();
    auto pEnd = points.getLast();
    
    int xRange = pEnd.x - pStart.x;
    float increment = (float)xRange / (float)resolution;
    
    Array<juce::Point<double>> dpoints;
    if (points.size() >= 3) {
        for (auto p : points)
            dpoints.add ({ double (p.getX()), double (p.getY())});
    }
    else {
        dpoints.add({double (pStart.getX()), double (pStart.getY())});
        
        // add a mid point so that we have over 3 points to satisfy the Spline interpolator
        int yRange = pEnd.y - pStart.y;
        double midX = (double)pEnd.getX() - ((double)xRange / 2.0);
        double midY = (double)pEnd.getY() - ((double)yRange / 2.0);
        
        dpoints.add({midX, midY});
        dpoints.add({double (pEnd.getX()), double (pEnd.getY())});
    }
    
    outputValues.clearQuick();
    Spline spline (dpoints);
    double bottom = (double)yLimit;
    if(bottom == 0) { return outputValues; }
    
    for (float x = (float)points.getFirst().getX(); x < (float)points.getLast().getX(); x += increment)
    {
        if(outputValues.size() >= resolution) {
            break;
        }
        
        double y = spline.interpolate (x);
        if(y < 0.0) {
            y = 0.0;
        }
        else if(y > bottom) {
            y = bottom;
        }
        
        outputValues.add((bottom - float(y)) / bottom);
    }
    
    return outputValues;
}

}
