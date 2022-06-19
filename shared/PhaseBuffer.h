#ifndef PhaseBuffer_h
#define PhaseBuffer_h

class PhaseBuffer {
public:
    PhaseBuffer(int clientCount, int initialSize) : m_clientCount(clientCount), m_resizeRequestCount(0), m_usePvoc(false) {
        prevPhase.resize(initialSize, 0.f);
    }
    
    bool isAvailable() {
        return m_usePvoc && m_resizeRequestCount == 0;
    }
    
    void requestResize(int newSize) {
        if (prevPhase.size() == newSize) {
            m_resizeRequestCount = 0;
            return;
        }

        m_resizeRequestCount++;
        
        if(m_resizeRequestCount == m_clientCount) {
            prevPhase.resize(newSize, 0.f);
            m_resizeRequestCount = 0;
        }
    }
    
    void setUsePvoc(bool shouldUsePvoc) {
        m_usePvoc = shouldUsePvoc;
        m_resizeRequestCount = 0;
    }
    
    // The overlapping instances of STFT will all write their previous phase values to this static array.
    std::vector<float> prevPhase;
    
private:
    const int m_clientCount;
    int m_resizeRequestCount;
    bool m_usePvoc;
};

#endif /* PhaseBuffer_h */
