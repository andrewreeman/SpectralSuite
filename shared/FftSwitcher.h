/*
  ==============================================================================

    FftSwitcher.h
    Created: 30 Nov 2018 11:38:32pm
    Author:  rem_d

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class FftSwitcherThread: public Thread {
public:
	class FftSwitcher {
	public:
		virtual void switchFftSize() = 0;
	};

	FftSwitcherThread(FftSwitcher* fftSwitcher) : Thread("fftSwitcherThread", 0), m_fftSwitcher(fftSwitcher) {}			

	void run() override {
		//this->sleep(5000);
		m_fftSwitcher->switchFftSize();
	}	

private:
	FftSwitcher* m_fftSwitcher;
};
