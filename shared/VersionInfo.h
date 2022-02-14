/*
  ==============================================================================

    VersionInfo.h
    Created: 1 Dec 2018 2:22:57pm
    Author:  rem_d

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class VersionInfo {
public:
	VersionInfo(String versionName, StringArray releaseNotes, String downloadUrl) 
		: m_versionName(versionName), m_releaseNotes(releaseNotes), m_downloadUrl(downloadUrl) {}

	String getVersionName() const { return m_versionName; }
	StringArray getReleaseNotes() const { return m_releaseNotes; }
	String getDownloadUrl() const { return m_downloadUrl; }
private:
	const String m_versionName;
	const StringArray m_releaseNotes;
	const String m_downloadUrl;
};