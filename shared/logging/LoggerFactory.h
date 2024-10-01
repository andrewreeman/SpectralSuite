#pragma once
#include "JuceHeader.h"
#include "NullLogger.h"

// LoggerRef only exists to clear up the current Logger when destroyed
class LoggerRef {
    public:
    LoggerRef(Logger* logger) : m_loggerRef(logger) {}
    
    Logger* get() const { return m_loggerRef.get(); };
    ~LoggerRef() {
        Logger::setCurrentLogger(nullptr);
    }
    
    private:
    std::unique_ptr<Logger> m_loggerRef;
    
    
};

class LoggerFactory {
    public:
    static File getLoggerFolder() {
        return FileLogger::getSystemLogFileFolder().getChildFile("SpectralSuite");
    }
    
    static LoggerRef* createLoggerReference() {
#if (JUCE_DEBUG)
        auto fileName = (String("SpectralSuite_") + ProjectInfo::projectName).replace(" ", "") + ".log";
        auto welcome = String("Start of spectral suite log for ") + ProjectInfo::projectName + ", version: " + ProjectInfo::versionString;
        auto fileLogger = FileLogger::createDefaultAppLogger("SpectralSuite", fileName, welcome, 1024 * 1024 * 30); // 30mb
        auto loggerRef = new LoggerRef(fileLogger);
#else
        auto loggerRef = new LoggerRef(new NullLogger());
#endif
        
        Logger::setCurrentLogger(loggerRef->get());
        return loggerRef;
    }
};
