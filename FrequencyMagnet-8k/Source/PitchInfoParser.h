#pragma once

#include <string>
#include <vector>

namespace PitchInfoParser {

struct PitchInfo {
    float frequency;
    int midiNote;
    std::string pitchName;
    
    bool isEmpty() {
        return pitchName.empty();
    }
};

float parseFrequency(std::string string);
float getFrequency(int midiNote);
std::string generateLabel(float frequency);
std::vector<PitchInfo> getPitchInfoList();
}
