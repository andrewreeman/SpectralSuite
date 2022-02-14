/*
  ==============================================================================

    VersionCheck.h
    Created: 1 Dec 2018 2:12:26pm
    Author:  rem_d

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "VersionInfo.h"

class VersionCheckThread : public Thread {
public:	
	class Listener {
	public:
		virtual void onNewVersionAvailable(VersionInfo* versionInfo) = 0;
        virtual ~Listener(){};
	};

	VersionCheckThread(int currentVersionCode, String versionCheckUrl) : Thread("versionCheckThread", 0),  m_currentVersionCode(currentVersionCode), m_versionCheckUrl(versionCheckUrl), m_listener(nullptr) {};
	void setListener(Listener* listener) { m_listener = listener; }
	void run() override;

private:
	const int m_currentVersionCode;
	const String m_versionCheckUrl;
	Listener* m_listener;
	
};
