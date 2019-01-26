#ifndef VERSIONCHECKER_H
#define VERSIONCHECKER_H

#include <curl/curl.h>
#include <string>
#include "vstgui/vstgui.h"
#include "editorshowupdateinterface.h"
#include "vstgui/lib//cvstguitimer.h"

const std::string VERSION_NUMBER = "1.12";
const std::string VERSION_URL = "http://www.andrewreeman.com/services/specSuiteVersion.php";

class VersionChecker : public CBaseObject{
public:
    VersionChecker(std::string url, std::string versionNumber);
    ~VersionChecker(){
		if (m_timer) m_timer->forget();
    }

    void checkForUpdates();

    virtual CMessageResult notify (CBaseObject* sender, const char* message);

    void setEditor(EditorShowUpdateInterface* editor){m_editor = editor;}
    EditorShowUpdateInterface* editor(){return m_editor;}

private:

    CURL* initCurl();
    void cleanup(CURL*);
    static size_t WriteMemoryCallback(void* contents, size_t size, size_t memb, void* userp);

    std::string m_url;
    std::string m_versionNumber;
    std::string m_body;
    EditorShowUpdateInterface* m_editor;
    CVSTGUITimer* m_timer;

};


#endif // VERSIONCHECKER_H
