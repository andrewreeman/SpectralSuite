#pragma once

#include "JuceHeader.h"

class FftSwitcherThread: public Thread {
public:
	class FftSwitcher {
	public:
		virtual void switchFftSize() = 0;
        virtual void switchOverlapCount() = 0;
        virtual void switchFftStyle() = 0;
        virtual void switchFftWindowType() = 0;
        virtual ~FftSwitcher(){};
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
    bool m_switchFft;
    bool m_switchFftStyle;
    bool m_switchOverlaps;
    bool m_switchWindow;
    
};
