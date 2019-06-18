#pragma once

#include "JuceHeader.h"

class FftSwitcherThread: public Thread {
public:
	class FftSwitcher {
	public:
		virtual void switchFftSize() = 0;
        virtual ~FftSwitcher(){};
	};

	FftSwitcherThread(FftSwitcher* fftSwitcher) : Thread("fftSwitcherThread", 0), m_fftSwitcher(fftSwitcher) {}			

	void run() override {
		//this->sleep(5000);
		m_fftSwitcher->switchFftSize();
	}	

private:
	FftSwitcher* m_fftSwitcher;
};
