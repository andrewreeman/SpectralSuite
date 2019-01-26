/*
  ==============================================================================

    VersionCheck.cpp
    Created: 1 Dec 2018 2:12:26pm
    Author:  rem_d

  ==============================================================================
*/

#include "VersionCheck.h"

void VersionCheckThread::run()
{		
	ScopedPointer<InputStream> urlInput = URLInputSource(m_versionCheckUrl).createInputStream();	
	if (urlInput == nullptr) { return; }

	String jsonString = urlInput->readString();
	auto parsedJson = JSON::fromString(jsonString);	

	String versionName = parsedJson.getProperty("version", String::empty).toString();
	if (versionName.isEmpty()) {
		DBG("Error parsing version name");
		return;
	}

	int code = (int)parsedJson.getProperty("code", -1);
	if (code == -1) {
		DBG("Error parsing version code");
		return;
	}
	
	String releaseUrl = parsedJson.getProperty("release_url", String::empty).toString();
	if (releaseUrl.isEmpty()) {
		DBG("Error parsing release url");
		return;
	}

	Array<var>* releaseNotesJsonArray = parsedJson.getProperty("release_notes", "[]").getArray();
	StringArray releaseNotes;
	for (var releaseNote : *releaseNotesJsonArray) {
		if (releaseNote.isString()) {
			releaseNotes.add(releaseNote.toString());
		}
	}

	if (code <= m_currentVersionCode) { return; }
	if (m_listener == nullptr) { return; }
				
	
	m_listener->onNewVersionAvailable(std::make_unique<VersionInfo>(versionName, releaseNotes, releaseUrl));
}
