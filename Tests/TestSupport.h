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

    // Minimal concrete AudioProcessor so parameter classes can be constructed
    // in headless tests. Derive from it if a test needs to override behaviour.
    class MockProcessor : public AudioProcessor
    {
    public:
        MockProcessor()
            : AudioProcessor (BusesProperties()
                                  .withInput ("In", AudioChannelSet::stereo(), true)
                                  .withOutput ("Out", AudioChannelSet::stereo(), true)) {}

        const String getName() const override { return "Mock"; }
        void prepareToPlay (double, int) override {}
        void releaseResources() override {}
        bool isBusesLayoutSupported (const BusesLayout&) const override { return true; }
        void processBlock (AudioBuffer<float>&, MidiBuffer&) override {}
        double getTailLengthSeconds() const override { return 0.0; }
        bool acceptsMidi() const override { return false; }
        bool producesMidi() const override { return false; }
        AudioProcessorEditor* createEditor() override { return nullptr; }
        bool hasEditor() const override { return false; }
        int getNumPrograms() override { return 1; }
        int getCurrentProgram() override { return 0; }
        void setCurrentProgram (int) override {}
        const String getProgramName (int) override { return {}; }
        void changeProgramName (int, const String&) override {}
        void getStateInformation (MemoryBlock&) override {}
        void setStateInformation (const void*, int) override {}
    };

    inline AudioBuffer<float> makeSineBuffer (int channels, int samples, int offset)
    {
        AudioBuffer<float> buffer (channels, samples);
        for (int ch = 0; ch < channels; ++ch)
            for (int i = 0; i < samples; ++i)
                buffer.setSample (ch, i, 0.5f * std::sin (0.05f * (float) (offset * samples + i)));
        return buffer;
    }

    inline bool allFinite (const AudioBuffer<float>& buffer)
    {
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
            for (int i = 0; i < buffer.getNumSamples(); ++i)
                if (! std::isfinite (buffer.getSample (ch, i)))
                    return false;
        return true;
    }

    inline bool allFinite (const std::vector<std::vector<float>>& buffer)
    {
        for (const auto& channel : buffer)
            for (float sample : channel)
                if (! std::isfinite (sample))
                    return false;
        return true;
    }

    inline bool allZero (const AudioBuffer<float>& buffer)
    {
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
            for (int i = 0; i < buffer.getNumSamples(); ++i)
                if (buffer.getSample (ch, i) != 0.f)
                    return false;
        return true;
    }
}
