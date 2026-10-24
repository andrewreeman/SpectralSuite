#include <JuceHeader.h>
#include <iostream>

// Entry point for the SpectralSuite acceptance tests.
// Runs every registered juce::UnitTest and exits non-zero if any expectation failed.

int main()
{
    ScopedJuceInitialiser_GUI juceInitialiser;

    UnitTestRunner runner;
    runner.setPassesAreLogged (false);
    runner.runAllTests();

    int failures = 0;

    for (int i = 0; i < runner.getNumResults(); ++i)
        if (const auto* result = runner.getResult (i))
            failures += result->failures;

    if (failures == 0)
        std::cout << "\nAll SpectralSuite tests passed." << std::endl;
    else
        std::cout << "\nSpectralSuite tests FAILED with " << failures << " failed expectation(s)." << std::endl;

    return failures == 0 ? 0 : 1;
}
