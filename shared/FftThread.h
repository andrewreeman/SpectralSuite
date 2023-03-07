#pragma once

#include "JuceHeader.h"

//TODO this class can be removed as it is unused
class FftThread : public Thread {
public:
    class FftWorker {
    public:
        virtual bool isWaitingForFft() = 0;
        virtual void performFftWork() = 0;
        virtual ~FftWorker(){};
    };
        
    FftThread() : Thread("fftThread", 0) {
        this->setPriority(Thread::Priority::high);
    }        
    
    void run() override;
    void addWorker(FftWorker* worker) {
        m_workers.push_back(worker);        
    }
    void removeWorker(FftWorker* worker);    
private:
    std::vector<FftWorker*> m_workers;
};
