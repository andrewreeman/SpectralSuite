// TODO: check if this class is unused

#include "FftThread.h"

void FftThread::run() {
   while(!threadShouldExit()) {
       // TODO: this should not be indefinite. The thread should only be started
       // when the fft size is changed or set to something high
       // then we will only be waiting when we know the thread should be active
       // TODO: also see what happens when user stops playback?? stop the thread I guess?
       if(!wait(-1)) {
           return;
       }
                                   
       
       for(auto w : m_workers) {
           if(threadShouldExit()) {
               return;
           }
           
           if(w->isWaitingForFft()) {
               w->performFftWork();
           }
                           
           yield();
       }
   }
};

void FftThread::removeWorker(FftWorker* worker) {
    // Lazy...removing all workers instead as we know that this will eventually be called by all destructors
    m_workers.clear();
}
