#pragma once

#include "JuceHeader.h"
#include "StandardFFTProcessor.h"
#include "PluginParameters.h"
#include "PhaseBuffer.h"
#include "FftWindowType.h"

#include <atomic>

class SpectralAudioPlugin;

class SpectralAudioProcessorInteractor {
public:
    SpectralAudioProcessorInteractor(int numOverlaps);
    virtual ~SpectralAudioProcessorInteractor(){}

    int getHopSize() const { return m_fftHopSize; }	
    int getFftSize();
    int getSampleRate() const { return m_sampleRate; }
    int getNumOverlaps() const { return m_numOverlaps; }
    std::shared_ptr<PhaseBuffer> getPhaseBuffer() const { return m_phaseBuffer; }

    // Serialises audio-thread processing against background FFT mutations.
    // The audio thread (SpectralAudioPlugin::processBlock) only try-locks this
    // and emits silence on contention; the FFT switcher thread and
    // prepareToPlay take it blocking while m_spectralProcess, the phase
    // buffer or the hop size are being changed. Mutating methods on this
    // class (setFftSize, setNumOverlaps, setWindowType, prepareToPlay) do not
    // lock: their callers own the locking protocol above.
    juce::CriticalSection& getProcessLock() { return m_processLock; }

    virtual std::unique_ptr<StandardFFTProcessor> createSpectralProcess(
		    int index, 
		    int fftSize,
		    int hopSize,
		    int sampleRate,
		    int numOverlaps,
		    int channel,
		    int channelCount) = 0;

    virtual void receivedMidi(MidiBuffer&){}
    virtual void prepareProcess(StandardFFTProcessor*) {}	
    virtual void onFftSizeChanged() {}		
	
    /// Can be overridden if needs to query plugin
    virtual void process(
		    SpectralAudioPlugin*,
		    std::vector<std::vector<float>>* input,
		    std::vector<std::vector<float>>* output)
    {
	    this->process(input, output);
    }

    virtual void process(std::vector<std::vector<float>>* input, std::vector<std::vector<float>>* output);

    void prepareToPlay(int fftSize, int sampleRate, int channelCount);
    void setFftSize(int fftSize);
    void usePvoc(bool usePvoc) { m_phaseBuffer->setUsePvoc(usePvoc); }
    void setNumOverlaps(int newOverlapCount);
    void setWindowType(FftWindowType newWindowType);
    // Set from the audio thread and read from the FFT switcher thread, so
    // these are atomic; they are advisory ("refuse changes while playing"),
    // with the process lock providing the hard exclusion.
    bool isPreparingToPlay() const { return m_isPreparingToPlay; }
    bool isPlaying() const { return m_isPlaying; }

protected:	
	std::vector< std::vector<std::unique_ptr<StandardFFTProcessor>> > m_spectralProcess;

private:
    int m_numOverlaps;
    int m_sampleRate;
    int m_fftHopSize;
    int m_numChans;
    int m_fftSize;
    std::atomic<bool> m_isPreparingToPlay;
    std::atomic<bool> m_isPlaying;
    juce::CriticalSection m_processLock;
    
    std::shared_ptr<PhaseBuffer> m_phaseBuffer;
};
