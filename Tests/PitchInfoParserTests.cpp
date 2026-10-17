#include <JuceHeader.h>
#include "TestSupport.h"

#include "FrequencyMagnet/Source/PitchInfoParser.h"

// Ported from FrequencyMagnet/Source/testParser.cpp.
class PitchInfoParserTests : public UnitTest
{
public:
    PitchInfoParserTests() : UnitTest ("PitchInfoParser", "SpectralSuite") {}

    void runTest() override
    {
        testGenerateLabel();
        testParseFrequency();
        testMidiToFrequency();
    }

private:
    void testGenerateLabel()
    {
        beginTest ("known frequencies generate the expected note labels");
        expectEquals (String (PitchInfoParser::generateLabel (1000.f)), String ("1000Hz (B5)"));
        expectEquals (String (PitchInfoParser::generateLabel (440.f)), String ("440Hz (A4)"));
        expectEquals (String (PitchInfoParser::generateLabel (27.f)), String ("27Hz (A0)"));
    }

    void testParseFrequency()
    {
        beginTest ("labels round-trip back to their integer frequency");
        expectEquals ((int) PitchInfoParser::parseFrequency ("1000Hz (B5)"), 1000);
        expectEquals ((int) PitchInfoParser::parseFrequency ("440Hz (A4)"), 440);
        expectEquals ((int) PitchInfoParser::parseFrequency ("27Hz (A0)"), 27);

        beginTest ("unparseable input falls back to 440");
        expectWithinAbsoluteError (PitchInfoParser::parseFrequency ("not a frequency"), 440.f, sstest::kEpsilon);
    }

    void testMidiToFrequency()
    {
        beginTest ("midi notes map to the expected frequencies");
        expectEquals (static_cast<int> (PitchInfoParser::getFrequency (83)), 987);
        expectEquals (static_cast<int> (PitchInfoParser::getFrequency (69)), 440);
        expectEquals (static_cast<int> (PitchInfoParser::getFrequency (21)), 27);
    }
};

static PitchInfoParserTests pitchInfoParserTests;
