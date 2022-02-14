#pragma once
#include <algorithm>

class TransitionState {
    enum States {
        OFF,
        START_TRANSITION,
        TRANSITION_IN,
        HOLD,
        TRANSITION_OUT
    };
    
    
public:
    TransitionState(int sampleRate, int blockSize) :
        m_sampleRate(sampleRate), m_incrementAmount(blockSize),
        m_state(OFF), m_inDurationSeconds(1), m_outDurationSeconds(1),
        m_counter(0.f), m_end(sampleRate * blockSize)
    {}

    bool isOff() const { return m_state == OFF; }
    bool shouldStartTransition() const { return m_state == START_TRANSITION; }
    bool isTransitioningIn() const { return m_state == TRANSITION_IN; }
    bool isTransitioningOut() const { return m_state == TRANSITION_OUT; }
    
    void setSampleRateAndBlockSize(int sampleRate, int blockSize);
    void setDuration(int newDuration){
        m_inDurationSeconds = newDuration;
        m_outDurationSeconds = newDuration;
    }
    
    void start();
    void stop();
    
    float next();
    
private:
    void tick();
    float value() const;
    int m_sampleRate;
    int m_incrementAmount;
    
    States m_state;
    
    int m_inDurationSeconds;
    int m_outDurationSeconds;
        
    float m_counter;
    float m_end;
    
};
