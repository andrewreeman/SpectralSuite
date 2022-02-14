#pragma once

#include "JuceHeader.h"

class FftSwitcherThread: public Thread {
public:
	class FftSwitcher {
	public:
		virtual void switchFftSize() = 0;
        virtual void switchOverlapCount() = 0;
        virtual ~FftSwitcher(){};
	};

	FftSwitcherThread(FftSwitcher* fftSwitcher)
        : Thread("fftSwitcherThread", 0), m_fftSwitcher(fftSwitcher), m_switchFft(false), m_switchOverlaps(false) {}
    
    /// Thread
    /// Do not call this directly. Instead call `switchFftSize` or `switchOverlapCount`
	void run() override {
        if(m_switchFft) {
            m_switchFft = false;
            m_fftSwitcher->switchFftSize();
        }
        else if(m_switchOverlaps) {
            m_switchOverlaps = false;
            m_fftSwitcher->switchOverlapCount();
        }
	}
 
    // public
    void switchFftSize() {
        m_switchFft = true;
        startThread();
    }
    
    void switchOverlapCount() {
        m_switchOverlaps = true;
        startThread();
    }

private:
	FftSwitcher* m_fftSwitcher;
    bool m_switchFft;
    bool m_switchOverlaps;
    
};
