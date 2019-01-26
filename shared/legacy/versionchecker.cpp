#include "versionchecker.h"
#include <stdexcept>
#include "vstgui/lib/cvstguitimer.h"

VersionChecker::VersionChecker(std::string url, std::string versionNumber)  : m_url(url), m_versionNumber(versionNumber), m_body(""){};

void VersionChecker::checkForUpdates()
{
    CURLcode response;
    std::string stringData = "";
    CURL* curlHandle = initCurl();

    response = curl_easy_perform(curlHandle);

    //TODO custom exception
    if(response != CURLE_OK){
        char errorStr[80];
        sprintf(errorStr, "Error during curl_easy_perform(CURL* curl). Response code: %d", response);
        cleanup(curlHandle);
        throw std::runtime_error(errorStr);
    }
    cleanup(curlHandle);
    if(m_versionNumber.compare(m_body)){
        m_timer = new CVSTGUITimer(this, 10000);
        m_timer->start();
        m_editor->showUpdate();
    };
}

CURL* VersionChecker::initCurl()
{
    curl_global_init(CURL_GLOBAL_ALL);
    CURL* curlHandle = curl_easy_init();
    curl_easy_setopt(curlHandle, CURLOPT_URL, m_url.c_str());
    curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, (void*)&m_body);
    curl_easy_setopt(curlHandle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    return curlHandle;
}

void VersionChecker::cleanup(CURL* curlHandle)
{
    curl_easy_cleanup(curlHandle);
    curl_global_cleanup();
}


size_t VersionChecker::WriteMemoryCallback(void* contents, size_t size, size_t memb, void* userp)
{
    size_t realsize = size*memb;
    char* charContents = (char*)contents;
    std::string* stringData = (std::string*)userp;

    for(int i=0; i<realsize; ++i){
        stringData->push_back(charContents[i]);
    }
    return realsize;
}

CMessageResult VersionChecker::notify(CBaseObject* sender, const char* message)
{
    CVSTGUITimer* timer = (CVSTGUITimer*)sender;
    timer->stop();
    m_editor->closeUpdate();
    return kMessageNotified;
}
