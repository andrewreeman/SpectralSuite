#pragma once

class LockState {
    enum States {
        CHANGE_TO_ON,
        ON,
        OFF,
    };
    
    States m_state;
public:
    LockState() : m_state(OFF) {}
    
    bool isOn() const { return m_state == ON; }
    bool isOff() const { return m_state == OFF; }
    bool shouldTransitionToOn() const { return m_state == CHANGE_TO_ON; }
   
    void off() { m_state = OFF; }
    void reset();
    void beginTransitionToOn();
    void completeTransitionToOn() { m_state = ON; }
};
