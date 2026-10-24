#include <JuceHeader.h>
#include "TestSupport.h"

#include "shared/SpectralAudioPlugin.h"
#include "shared/SpectralAudioProcessorInteractor.h"
#include "BinScrambler/Source/BinScramblerParameters.h"
#include "BinScrambler/Source/BinScramblerInteractor.h"

#include <thread>

// Project save/reload through the real SpectralAudioPlugin: state saved at a
// non-default FFT size is restored into a fresh plugin the way hosts do it,
// then audio is processed. Output must stay finite, must not throw, and must
// not be silent once the plugin's latency has passed.

namespace
{
    using sstest::allFinite;
    using sstest::makeSineBuffer;

    class ReloadTestFactory : public SpectralAudioPlugin::DependencyFactory
    {
    public:
        std::shared_ptr<PluginParameters> createParams (SpectralAudioPlugin* plugin) override
        {
            m_params = std::make_shared<BinScramblerParameters> (plugin);
            return m_params;
        }

        ParameterContainerComponent* createUi (SpectralAudioPlugin*) override { return nullptr; }

        std::unique_ptr<SpectralAudioProcessorInteractor> createProcessor (SpectralAudioPlugin*) override
        {
            auto interactor = std::make_unique<BinScramblerInteractor> (4, m_params);
            m_interactor = interactor.get();
            return interactor;
        }

        std::shared_ptr<BinScramblerParameters> m_params;
        SpectralAudioProcessorInteractor* m_interactor = nullptr;
    };

    AudioParameterChoice* fftParameter (AudioProcessor& plugin)
    {
        for (auto* p : plugin.getParameters())
            if (auto* withId = dynamic_cast<AudioProcessorParameterWithID*> (p))
                if (withId->getParameterID() == "fft")
                    return dynamic_cast<AudioParameterChoice*> (p);
        return nullptr;
    }

    struct RunResult
    {
        bool threw = false;
        String exception;
        bool finite = true;
        float peak = 0.f;
    };

    // Feeds sine blocks; the switcher thread works asynchronously, so give it
    // time between blocks rather than racing it.
    RunResult run (SpectralAudioPlugin& plugin, int blocks, int& blockIndex)
    {
        RunResult result;
        MidiBuffer midi;
        try
        {
            for (int i = 0; i < blocks; ++i)
            {
                auto buffer = makeSineBuffer (2, 64, blockIndex++);
                plugin.processBlock (buffer, midi);
                result.finite = result.finite && allFinite (buffer);
                result.peak = jmax (result.peak, buffer.getMagnitude (0, buffer.getNumSamples()));
                if (i % 16 == 0)
                    std::this_thread::sleep_for (std::chrono::milliseconds (1));
            }
        }
        catch (const std::exception& e)
        {
            result.threw = true;
            result.exception = e.what();
        }
        catch (...)
        {
            result.threw = true;
            result.exception = "unknown exception";
        }
        return result;
    }

    // A project saved with the FFT size choice at `fftIndex`.
    MemoryBlock savedProjectState (int fftIndex)
    {
        SpectralAudioPlugin plugin (new ReloadTestFactory()); // the plugin owns the factory
        plugin.prepareToPlay (48000.0, 64);
        *fftParameter (plugin) = fftIndex;
        int blockIndex = 0;
        run (plugin, 400, blockIndex);
        MemoryBlock state;
        plugin.getStateInformation (state);
        return state;
    }
}

class ProjectReloadTests : public UnitTest
{
public:
    ProjectReloadTests() : UnitTest ("Project reload at non-default FFT sizes", "SpectralSuite") {}

    void runTest() override
    {
        // FFT size choices start at 128 (index 0); the default is index 4 (2048).
        struct Size { int index; int fft; };
        for (auto size : { Size { 3, 1024 }, Size { 4, 2048 }, Size { 5, 4096 } })
        {
            const auto state = savedProjectState (size.index);

            beginTest ("reload at FFT " + String (size.fft) + ": setState then prepareToPlay");
            {
                auto* factory = new ReloadTestFactory();
                SpectralAudioPlugin plugin (factory); // the plugin owns the factory
                plugin.setStateInformation (state.getData(), (int) state.getSize());
                plugin.prepareToPlay (48000.0, 64);
                expectEquals (fftParameter (plugin)->getIndex(), size.index);
                int blockIndex = 0;
                check (run (plugin, 1500, blockIndex));
                expectEquals (factory->m_interactor->getHopSize(), size.fft / 4, "hop size doesn't match the restored FFT size");
            }

            // Issue #97 regression in 2.3.0 testing: REAPER prepares at the default size before
            // restoring the project; the stale processor bank crashed Bin Scrambler below the
            // default size and silenced it above.
            beginTest ("reload at FFT " + String (size.fft) + ": prepareToPlay, setState, prepareToPlay");
            {
                auto* factory = new ReloadTestFactory();
                SpectralAudioPlugin plugin (factory); // the plugin owns the factory
                plugin.prepareToPlay (48000.0, 64);
                plugin.setStateInformation (state.getData(), (int) state.getSize());
                plugin.prepareToPlay (48000.0, 64);
                int blockIndex = 0;
                check (run (plugin, 1500, blockIndex));
                expectEquals (factory->m_interactor->getHopSize(), size.fft / 4, "hop size doesn't match the restored FFT size");
            }
        }
    }

private:
    void check (const RunResult& r)
    {
        expect (! r.threw, "processBlock threw: " + r.exception);
        expect (r.finite, "output contains non-finite samples");
        expect (r.peak > 1.0e-4f, "output is silent (peak " + String (r.peak) + ")");
    }
};

static ProjectReloadTests projectReloadTests;
