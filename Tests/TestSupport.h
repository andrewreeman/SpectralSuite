#pragma once

#include <JuceHeader.h>

#include "shared/Types.h"
#include "shared/Polar.h"

// Helpers shared by the SpectralSuite acceptance tests.

namespace sstest
{
    inline constexpr float kEpsilon = 1.0e-4f;

    inline bool almostEqual (float a, float b, float eps = kEpsilon)
    {
        return std::abs (a - b) <= eps;
    }
}
