#include "VersionCheck.h"

void VersionCheckThread::run()
{
    std::unique_ptr<InputStream> urlInput(URLInputSource(m_versionCheckUrl).createInputStream());
    if (urlInput == nullptr) { return; }

	String jsonString = urlInput->readString();
    if(this->threadShouldExit()) { return; }
    
	auto parsedJson = JSON::fromString(jsonString);	

	String versionName = parsedJson.getProperty("version", String()).toString();
	if (versionName.isEmpty()) {
		DBG("Error parsing version name");
		return;
	}

	int code = (int)parsedJson.getProperty("code", -1);
	if (code == -1) {
		DBG("Error parsing version code");
		return;
	}
	
	String releaseUrl = parsedJson.getProperty("release_url", String()).toString();
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
    if(this->threadShouldExit()) { return; }
    
    MessageManager::callAsync([versionName, releaseNotes, releaseUrl, this](){
        VersionInfo* versionInfo = new VersionInfo(versionName, releaseNotes, releaseUrl);
        m_listener->onNewVersionAvailable(versionInfo);
        delete versionInfo;
    });
}
