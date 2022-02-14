#include "LockState.h"


/// MARK: state definitions
void LockState::reset() {
    if (m_state == CHANGE_TO_ON || m_state == ON) {
        m_state = CHANGE_TO_ON;
    }
    else {
        m_state = OFF;
    }
}
        
void LockState::beginTransitionToOn() {
    if (m_state  == OFF) {
        m_state = CHANGE_TO_ON;
    }
}
