/*
  ==============================================================================

    SplineHelper.h
    Created: 28 Aug 2023 6:02:16pm
    Author:  Andrew Reeman

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

namespace SplineHelper {
    Array<float> getAudioSplineValues(Array<juce::Point<int>> points, float yLimit);
}
