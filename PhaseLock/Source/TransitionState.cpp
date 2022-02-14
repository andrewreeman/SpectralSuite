#include "TransitionState.h"

 void TransitionState::start() {
    if(!isOff()) { return; }

    m_end = m_sampleRate * m_inDurationSeconds;
    m_counter = 0.0;
    m_state = START_TRANSITION;
}

void TransitionState::stop() {
    if(m_state == TRANSITION_OUT) { return; }

    if(m_state == TRANSITION_IN || m_state == HOLD) {
        m_state = TRANSITION_OUT;
    }
    else {
        m_state = OFF;
    }
}

float TransitionState::next() {
    float v = value();
    tick();
    return v;
}

void TransitionState::tick() {
    if(m_state == START_TRANSITION) {
        m_state = TRANSITION_IN;
    }

    if(m_state == TRANSITION_IN) {
        if(m_counter >= m_end) {
            m_state = HOLD;
        }
        else {
            m_counter += m_incrementAmount;
        }
    }
    else if(m_state == TRANSITION_OUT) {
        if(m_counter <= 0.0) {
            m_state = OFF;
        }
        else {
            m_counter -= m_incrementAmount;
        }
    }
}

float TransitionState::value() const {
    if(m_end <= 0.f) {
        return 0.f;
    }

    if(m_state == TRANSITION_IN) {
        return std::min<float>(1.f, m_counter / m_end);
    }
    else if(m_state == TRANSITION_OUT) {
        return std::max<float>(0.0, m_counter / m_end);
    }
    else if(m_state == HOLD){
        return 1.f;
    }
    else {
        return 0.f;
    }
}

void TransitionState::setSampleRateAndBlockSize(int sampleRate, int blockSize) {
    m_sampleRate = sampleRate;
    m_incrementAmount = blockSize;
    
    m_counter = 0.f;
    m_state = OFF;
}
