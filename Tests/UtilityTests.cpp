#include <JuceHeader.h>
#include "TestSupport.h"

#include "shared/utilities.h"

class UtilityTests : public UnitTest
{
public:
    UtilityTests() : UnitTest ("Utilities", "SpectralSuite") {}

    void runTest() override
    {
        testClipAndWrap();
        testInterpolation();
        testAngleWrapping();
        testPolarRoundTrip();
        testVectorScramble();
    }

private:
    void testClipAndWrap()
    {
        beginTest ("clip clamps to the inclusive range");
        expectEquals (utilities::clip (5, 0, 10), 5);
        expectEquals (utilities::clip (-1, 0, 10), 0);
        expectEquals (utilities::clip (11, 0, 10), 10);

        beginTest ("wrap handles negatives and the limit");
        expectEquals (utilities::wrap (3, 10), 3);
        expectEquals (utilities::wrap (-1, 10), 9);
        expectEquals (utilities::wrap (10, 10), 0);
    }

    void testInterpolation()
    {
        beginTest ("interp_lin interpolates between two values");
        expectWithinAbsoluteError (utilities::interp_lin (0.f, 10.f, 0.5f), 5.f, sstest::kEpsilon);
        expectWithinAbsoluteError (utilities::interp_lin (0.f, 10.f, 0.25f), 2.5f, sstest::kEpsilon);
        // a factor of zero on the integer part returns the low value
        expectWithinAbsoluteError (utilities::interp_lin (0.f, 10.f, 1.0f), 0.f, sstest::kEpsilon);
        // negative fractional part is inverted back into range
        expectWithinAbsoluteError (utilities::interp_lin (0.f, 10.f, -0.5f), 5.f, sstest::kEpsilon);

        beginTest ("interp_lin with explicit index bounds");
        expectWithinAbsoluteError (utilities::interp_lin (0.f, 10.f, 0, 1, 0.5f), 5.f, sstest::kEpsilon);
        expectWithinAbsoluteError (utilities::interp_lin (0.f, 10.f, 0, 1, 0.0f), 0.f, sstest::kEpsilon);
        // above the high bound returns the high value
        expectWithinAbsoluteError (utilities::interp_lin (0.f, 10.f, 0, 1, 2.0f), 10.f, sstest::kEpsilon);

        beginTest ("interp_lin_normalised behaves like a 0..1 interpolation");
        expectWithinAbsoluteError (utilities::interp_lin_normalised (0.f, 10.f, 0.5f), 5.f, sstest::kEpsilon);
        expectWithinAbsoluteError (utilities::interp_lin_normalised (10.f, 0.f, 0.25f), 7.5f, sstest::kEpsilon);
    }

    void testAngleWrapping()
    {
        beginTest ("wrap_twopi folds angles into a single turn");
        float small = 1.0f;
        utilities::wrap_twopi (small);
        expectWithinAbsoluteError (small, 1.0f, sstest::kEpsilon);

        float big = 7.0f;
        utilities::wrap_twopi (big);
        expectWithinAbsoluteError (big, 7.0f - 6.28318531f, sstest::kEpsilon);
    }

    void testPolarRoundTrip()
    {
        beginTest ("car2Pol then pol2Car recovers the original bins");
        std::vector<Cpx> cpx { { 3.f, 4.f }, { -1.f, 2.f }, { 0.5f, -0.5f }, { 0.f, 0.f } };
        std::vector<Cpx> recovered (cpx.size());

        PolarVector polar (2);
        utilities::car2Pol (cpx, polar, 2);
        expectWithinAbsoluteError (polar[0].m_mag, 5.f, sstest::kEpsilon);
        expectWithinAbsoluteError (polar[1].m_mag, std::sqrt (5.f), sstest::kEpsilon);

        utilities::pol2Car (polar, recovered, 2);
        expectWithinAbsoluteError (recovered[0].real(), 3.f, sstest::kEpsilon);
        expectWithinAbsoluteError (recovered[0].imag(), 4.f, sstest::kEpsilon);
        expectWithinAbsoluteError (recovered[1].real(), -1.f, sstest::kEpsilon);
        expectWithinAbsoluteError (recovered[1].imag(), 2.f, sstest::kEpsilon);
        // mirrored bins are zeroed by the reflection step
        expectWithinAbsoluteError (recovered[3].real(), 0.f, sstest::kEpsilon);
        expectWithinAbsoluteError (recovered[2].real(), 0.f, sstest::kEpsilon);
    }

    void testVectorScramble()
    {
        beginTest ("vector_Scramble with zero amount is the identity");
        std::vector<int> identity { 0, 1, 2, 3, 4, 5, 6, 7 };
        utilities::vector_Scramble (identity, 0);
        expect (identity == std::vector<int> { 0, 1, 2, 3, 4, 5, 6, 7 });

        beginTest ("vector_Scramble preserves size and elements");
        std::vector<int> input { 0, 1, 2, 3, 4, 5, 6, 7 };
        utilities::vector_Scramble (input, 2);
        expectEquals (static_cast<int> (input.size()), 8);

        auto sorted = input;
        std::sort (sorted.begin(), sorted.end());
        expect (sorted == std::vector<int> { 0, 1, 2, 3, 4, 5, 6, 7 });
    }
};

static UtilityTests utilityTests;
