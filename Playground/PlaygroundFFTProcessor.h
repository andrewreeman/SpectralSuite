#pragma once

#include "../shared/PhaseVocoder.h"

class PlaygroundFFTProcessor : public PhaseVocoder, public FftThread::FftWorker {
protected:
    int m_initialOffset;
    /* Rotate the input in preparation for finding the difference in phase between the ideal center frequency
    and the most prominant frequency content */
    void doHannRotate(std::vector<FftDecimal>& inOut) override;
    // unrotate for passing the data to the output
    void doHannUnrotate(const std::vector<Cpx>& inCpx, std::vector<FftDecimal>& outFloat) override;
    
public:

    PlaygroundFFTProcessor(int size, int hops, int offset, int sRate, std::shared_ptr<PhaseBuffer> _phaseBuffer, std::shared_ptr<FftThread> fftThread);
    ~PlaygroundFFTProcessor() {
        m_fftThread->removeWorker(this);
    }
        
    void setDoWindowRotate(bool doWindowRotate){ m_doWindowRotate = doWindowRotate; }
    void setUseThreadForLargeFft(bool useThread){ m_useThreadForLargeFft = useThread; }
    
    virtual bool setFFTSize(int newSize) override;
    virtual void fill_in_passOut(const FftDecimal* audioInput, FftDecimal* processOutput, int blockSize) override;
    virtual void process(const FftDecimal* input, FftDecimal* output, int blockSize) override;
            
    // still an abstract class that needs spectral_process to be overwritten
    virtual void spectral_process(const PolarVector &in, PolarVector &out, int bins) override {
         for(int i=0; i<bins; ++i){
            out[i] = in[i];
        }
    };
    
    
    
    bool isWaitingForFft() override { return m_isWaitingForFft; };
    void performFftWork() override;
    
    enum WindowType {
        HANN,
        HANN_LINEAR
    };
    
    void setWindowType(WindowType type) {
        m_windowType = type;
    }
    
private:
    bool isLargeFft() { return m_fftSize > 32768; }
    void readyForFft();
    void yield();
    
    std::shared_ptr<PhaseBuffer> phaseBuffer;
    bool m_doWindowRotate;
    bool m_isWaitingForFft;
    bool m_useThreadForLargeFft;
    WindowType m_windowType;
    
    
    std::vector<FftDecimal> m_prevOutput;
    std::shared_ptr<FftThread> m_fftThread;
    
    int m_stretchedOutputReadIndex;
    std::vector<Cpx> m_expandedInput;
    std::vector<Cpx> m_stretchedOutputCpx;
    std::vector<FftDecimal> m_stretchedOutput;
};

