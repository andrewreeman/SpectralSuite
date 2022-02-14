#include <iostream>
#include "PitchInfoParser.h"

// https://stackoverflow.com/a/9158263/1742518
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define BLUE    "\033[34m" 

void printPass(std::string message = "Pass") {
    std::cout << GREEN << message << RESET << std::endl;
}

void printStartOfTest() {
    std::cout << std::endl;
    std::cout << BLUE << "=== Begin test " << RESET;
}

void printEndOfTest() {
    std::cout << BLUE << "=== Test passed" << RESET << std::endl;
}

void printError(std::string error) {
    std::cout << RED << error << RESET << std::endl;
}

void test(float frequency, std::string expectedLabel) {
    printStartOfTest();

    int expectedFrequency = frequency;    
    std::cout << std::to_string(expectedFrequency) << ", " << expectedLabel << std::endl;    
    
    std::cout << "Testing PitchInfoParser that frequency " << std::to_string(expectedFrequency) 
        << " will produce label " 
        << std::endl;
        
    auto label = PitchInfoParser::generateLabel(frequency);        
    if(label != expectedLabel) {
        printError("Expected " + expectedLabel + " did not match received " + label);
        assert(label == expectedLabel);     
    }    
    printPass();           

    std::cout << "Testing PitchInfoParser will parse frequency " << std::to_string(expectedFrequency) << " if provided the retreived label" << std::endl;
    int parsedFreq = PitchInfoParser::parseFrequency(label);
    assert(parsedFreq == expectedFrequency);
    printPass();           
    printEndOfTest();    
}

void test(int midiNote, int expectedFrequency) {
    printStartOfTest();

    std::cout << std::to_string(midiNote) << ", " << std::to_string(expectedFrequency) << std::endl;
    std::cout << "Testing that PitchInfoParser will produce the expected frequency " 
        << std::to_string(expectedFrequency) 
        << " from being provided a midi number " 
        << std::to_string(midiNote) 
        << std::endl;
    
    int foundFreq = PitchInfoParser::getFrequency(midiNote);    
    std::cout << "Found frequency " << std::to_string(foundFreq) << std::endl;
    assert(foundFreq == expectedFrequency);
    printPass();           
    printEndOfTest();
}

int main() {    
    test(1000, "1000Hz (B5)");
    test(440, "440Hz (A4)");
    test(27, "27Hz (A0)");
    
    test(83, 987);
    test(69, 440);
    test(21, 27);

    return 0;
}