#pragma once
#include "JuceHeader.h"
#include "NullLogger.h"

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
    static LoggerRef* createLoggerReference() {
#if (JUCE_DEBUG)
        auto appData = File::getSpecialLocation(File::userApplicationDataDirectory);
        auto specSuiteLog = appData.getChildFile("SpectralSuite/spectral-suite-debug.log");
        auto loggerRef = new LoggerRef(new FileLogger(specSuiteLog, "Start of spectral suite log"));
#else
        auto loggerRef = new LoggerRef(new NullLogger());
#endif
        
        Logger::setCurrentLogger(loggerRef->get());
        return loggerRef;
    }
};
