#pragma once

#include "JuceHeader.h"

class NullLogger : public Logger {
    public:
        void logMessage (const String&) override {
            // Do nothing
        }
};
