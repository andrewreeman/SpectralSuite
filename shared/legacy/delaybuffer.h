/*-----

This class is a delay buffer that is used for holding a buffer that is continuously written to (in blocks)
and read from. The write and read functions both include a coefficient argument that will scale the input
and output accordingly.
As the delay buffer works in blocks it involves increment mechanisms for filling up the buffer.

Andrew Reeman - 01/08/2012
-----*/

// for this project a general header guard for all the general libraries is used.
#ifndef __include_libs__
#define __include_libs__
#include <cstring>
#include <complex>
#include "kissfft.hh"
#include <cmath>
#include <vector>
#include<algorithm>
#include <stdlib.h>
#include<time.h>
#include <fstream>
#endif

#ifndef __processes__
#include "../shared/lib/processes.h"
#endif

#ifndef __us_vst__
#include "us_vst.h"
#endif

#ifndef DELAYBUFFER_H
#define DELAYBUFFER_H



template<class T>
class DelayBuffer{

    public:
		//size in samples. The host block size sets block size for the write loops.
        DelayBuffer( int size, int hostBlockSize, /**/ int initTime);
        virtual ~DelayBuffer(){};

        int getSize() const{ return m_size; } //returns buffer size
        void setSize(int val);

		// can use vectors or pointers.
		// If delWrite receives only a pointer/vector and a T coefficient it will replace the input buffer in blocks
        void delWrite(const std::vector<T>& input, T coeff);
        void delWrite(const T* input, T coeff);

		/* If delWrite receives the above arguments along with an integer as the second argument then the writing will occur
		 at the index given by the integer argument. This is useful for external sample by sample writing */
        void delWrite(const std::vector<T>& input, int index, T coeff);
        void delWrite(const T* input, int index, T coeff);

		// instead of clearing the buffers we can also accumulate.
        void delAccum(const std::vector<T>& input, T coeff);
        void delAccum(const T* input, T coeff);

		// This will return the value at the index-delaySamps
        T delRead( int std_Index, int delaySamps,  T coeff) const;
        T delReadInterp(int std_Index, int delayTime, T coeff, Line& interpLine);

        void clearBuffer();
		// The next three routines are used for the block increments involved in delWrite
        void setBlockSize(int hostBlockSize);
        void incrementCount();

        void clip(std::vector<T>& inOut);

    private:

        std::vector<T> m_buffer;
        int m_size;
        int m_blockSize;
        int m_count; // when the count reaches m_size the buffer is full.
        bool m_setClip;

        // for future child class

        int m_prevTime;
        int m_curTime;

};

// definitions are in the header file due to the types being templates

template<class T>
DelayBuffer<T>::DelayBuffer(int size, int blockSize, /*for child */ int initTime) :
    m_size(size), m_buffer(size), m_blockSize(blockSize), m_count(0), m_setClip(false) /**/,
    m_curTime(initTime), m_prevTime(initTime)
{
}

 template<class T>
void DelayBuffer<T>::setSize(int newSize){
    m_size = newSize;
    m_buffer.resize(m_size, T(0));
}

template<class T>
void DelayBuffer<T>::setBlockSize(int hostBlockSize){
  m_blockSize = hostBlockSize;
}

template<class T>
void DelayBuffer<T>::incrementCount(){
    m_count += m_blockSize;
    m_count %= m_size; //wrap around when at the end of the buffer
}

template<class T>
void DelayBuffer<T>::delWrite(const std::vector<T>& input,  T coeff){

     //fill input buffer gradually in increments and wrap around if above the buffer size

     for(int i=0; i<m_blockSize; ++i){
        int buff_Index = (i+m_count)%m_size;
        m_buffer[buff_Index] = input[i]*coeff;
     }
}

template<class T>
void DelayBuffer<T>::delWrite(const T* input,  T coeff){
     for(int i=0; i<m_blockSize; ++i){
        int buff_Index = (i+m_count)%m_size;
        m_buffer[buff_Index] = input[i]*coeff;
     }
}

template<class T>
void DelayBuffer<T>::delWrite(const std::vector<T>&input, int index, T coeff){
    int buff_Index = (index+m_count)%m_size;
    m_buffer[buff_Index] = input[index]*coeff;
}


template<class T>
void DelayBuffer<T>::delWrite(const T* input, int index, T coeff){
    int buff_Index = (index+m_count)%m_size;
    m_buffer[buff_Index] = input[index]*coeff;
}

template<class T>
void DelayBuffer<T>::delAccum(const std::vector<T>& input,  T coeff){
    for(int i=0; i<m_blockSize; ++i){
        int buff_Index = (i+m_count)%m_size;
        m_buffer[buff_Index] += input[i]*coeff;
     }
}

template<class T>
void DelayBuffer<T>::delAccum(const T* input,  T coeff){
     for(int i=0; i<m_blockSize; ++i){
        int buff_Index = (i+m_count)%m_size;
        m_buffer[buff_Index] += input[i]*coeff;
     }
}

template<class T>
//T DelayBuffer<T>::delRead(int std_Index, int delaySamps = 0,  T coeff = T(0.4))const{
T DelayBuffer<T>::delRead(int std_Index, int delaySamps, T coeff)const {

	/* because the reading and writing should be happening continuously the read 'head'
	should be at the same location at the (write head - delaySamps)
	The read head should be wrapped around to the end of the read buffer if below zero */
    int index = processes::wrap(std_Index-delaySamps, m_size);

    T val = m_buffer[(index+m_count)%m_size];
    return val*coeff;
}

template<class T>
void DelayBuffer<T>::clearBuffer(){
    std::fill(m_buffer.begin(), m_buffer.end(), T(0));
}

template<class T>
T DelayBuffer<T>::delReadInterp(int std_Index, int delayTime, T coeff, Line& interpLine){

    float interpPos = interpLine.getPos();

	if(m_prevTime != delayTime){
	    T valA;
	    T valB;
	  //  T outVal;
         //interpolate between prevTime and delayTime
        valA = delRead(std_Index, m_prevTime, coeff);
        valB = delRead(std_Index, m_curTime, coeff);
    //    outVal = processes::interp_lin(valA, valB, interpLine);

        if(interpPos == 1.f){
            m_prevTime = m_curTime;
            m_curTime = delayTime;
            interpLine.trigger();
           //  return delRead(std_Index, m_curTime, coeff);
        }
        //

        //
        return valA * (1.f-interpPos) + valB * interpPos;
    }
    else{
        //
       // return 0.f;
        //
        return delRead(std_Index, delayTime, coeff);
    }
};



#endif // DELAYBUFFER_H
