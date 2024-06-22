#include "PitchInfoParser.h"
#include <cmath>
#include <stdexcept>

#ifdef DEBUG
    #include <iostream>
#endif

namespace PitchInfoParser {
float roundFreq(double var)
{
    // 37.66666 * 100 =3766.66
    // 3766.66 + .5 =3767.16    for rounding off value
    // then type cast to int so value is 3767
    // then divided by 100 so the value converted into 37.67
    float value = (int)(var * 100 + .5);
    return (float)value / 100;
}

std::string findNote(float frequency) {
    auto notes = getPitchInfoList();
    
    PitchInfo foundPitchInfo, previousPitchInfo;
    for(auto n : notes) {
        if(n.frequency >= frequency) {
            foundPitchInfo = n;
            break;
        }
        previousPitchInfo = n;
    }
    
    if(foundPitchInfo.isEmpty()) {
        return "?";
    }
    
    if(previousPitchInfo.isEmpty()) {
        return foundPitchInfo.pitchName;
    }        
    
    float lastPairDiff = frequency - previousPitchInfo.frequency;
    float foundPairDiff = foundPitchInfo.frequency - frequency;
    
    return foundPairDiff < lastPairDiff ? foundPitchInfo.pitchName : previousPitchInfo.pitchName;
}

float parseFrequency(std::string string) {
    try {
        return (float)std::stoi(string);
    } catch(std::invalid_argument invalidArg) {
        return 440.f;
    }
}

std::string generateLabel(float frequency) {
    return std::to_string((int)frequency) + "Hz (" + findNote(frequency) + ")";
}

float getFrequency(int midiNote) {
    float foundFrequency = -1;
    
    for(auto pitchInfo : getPitchInfoList()) {
        if(pitchInfo.midiNote == midiNote) {
            foundFrequency = pitchInfo.frequency;
        }
    }
    
    return foundFrequency;
}

std::vector<PitchInfo> getPitchInfoList() {
    static std::vector<std::string> NOTE_LIST = {
        "C", "C#", "D", "Eb", "E", "F", "F#", "G", "Ab", "A", "Bb", "B"
    };
    
    static std::vector<PitchInfo> PITCH_INFO_LIST;
    
    if(PITCH_INFO_LIST.empty()) {
        for(int octave=0; octave<=8; ++octave) {
            for(int note=0; note<NOTE_LIST.size(); ++note) {
                std::string noteString = NOTE_LIST[note] + std::to_string(octave-1);
                double noteNumber = (double)((octave * 12) + note);
                int midiNumber = noteNumber;
                double freq = 440.0 * pow(2.0,  (noteNumber - 69) / 12.0);
                
#ifdef DEBUG
                std::cout
                    << "Freq: " << std::to_string(roundFreq(freq))
                    << " Midi:" << std::to_string(midiNumber)
                    << " Note name:" << noteString
                    << std::endl;
#endif
                PITCH_INFO_LIST.push_back(PitchInfo{ roundFreq(freq), midiNumber, noteString  });
            }
        }
    }
        
    return PITCH_INFO_LIST;
}
}
