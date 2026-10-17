#pragma once

#include "JuceHeader.h"

// Performs a single queued FFT configuration change on its own thread. The
// request flags are written from the audio thread (processBlock) and consumed
// here, so they are atomic. Callers must not assume a request has run when
// switchX() returns; use isBusy() to poll.
class FftSwitcherThread: public Thread {
public:
	class FftSwitcher {
	public:
		virtual void switchFftSize() = 0;
        virtual void switchOverlapCount() = 0;
        virtual void switchFftStyle() = 0;
        virtual void switchFftWindowType() = 0;
        virtual ~FftSwitcher(){}
	};

	FftSwitcherThread(FftSwitcher* fftSwitcher)
        : Thread("fftSwitcherThread", 0), m_fftSwitcher(fftSwitcher), m_switchFft(false), m_switchFftStyle(false), m_switchOverlaps(false), m_switchWindow(false) {}
    
    /// Thread
    /// Do not call this directly. Instead call `switchFftSize` or `switchOverlapCount`
	void run() override {
        if (threadShouldExit()) { return; }
        
        if(m_switchFft) {
            m_switchFft = false;
            m_fftSwitcher->switchFftSize();
        }
        else if(m_switchOverlaps) {
            m_switchOverlaps = false;
            m_fftSwitcher->switchOverlapCount();
        }
        else if(m_switchFftStyle) {
            m_switchFftStyle = false;
            m_fftSwitcher->switchFftStyle();
        }
        else if(m_switchWindow) {
            m_switchWindow = false;
            m_fftSwitcher->switchFftWindowType();
        }
	}
 
    // public
    void switchFftSize() {
        m_switchFft = true;
        startThread();
    }
    
    void switchFftStyle() {
        m_switchFftStyle = true;
        startThread();
    }
    
    void switchOverlapCount() {
        m_switchOverlaps = true;
        startThread();
    }
    
    void switchWindowType() {
        m_switchWindow = true;
        startThread();
    }
    
    bool isBusy() {
        return m_switchFft || m_switchFftStyle || m_switchOverlaps || m_switchWindow || isThreadRunning();
    }

private:
	FftSwitcher* m_fftSwitcher;
    std::atomic<bool> m_switchFft;
    std::atomic<bool> m_switchFftStyle;
    std::atomic<bool> m_switchOverlaps;
    std::atomic<bool> m_switchWindow;
    
};
