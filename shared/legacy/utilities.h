/*-----

This library contains general functions and procedures that are used within the vst suite

-----*/

#pragma once
#ifndef PROCESSES_H
#define PROCESSES_H

#include <cstring>
#include <complex>
#include <cmath>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <cassert>


#ifndef __constants__
#define __constants__
#define ROOT2OVER2 0.70710678
#define HALFPI 1.570796325
#define TWOPI 6.28318531
#define PI 3.14159265
#endif

namespace utilities{

    enum{
       // vector randomise modes
       RAND_SCRAMBLE,
       RAND_SPRINKLE,
       //  wrap modes
       CONTINUE = 0,
       WRAP,
       REFLECT,
       CLIP
    };
		
	int ms2Samps(int ms, int sRate);					
	int wrap(int index, int limit);    

	template<class T>
	T clip(const T& inVal, T lowLim, T hiLim) {
		return std::min(std::max(inVal, lowLim), hiLim);
	}

	/* The following functions are defined in the header to avoid the need to specialise the templates.
    This enables the templates to be of any type.*/
	
    //interpolating between two values
    template <class T>
	T interp_lin(T a, T b, float index) {		
		int intPart = (int)index;
		float factor = index - intPart;

		// if negative then invert the factor
		if (factor < 0.0) {
			factor += 1.0;
		}

		float diff = (float)(b - a);
		return a + (diff * factor);
	}

    template<class T>
	void wrap_twopi(T &v) {
		v = fmodf(v, (float)TWOPI);		
	}
	
        
	template<class T>
	void vector_Scramble(std::vector<T>& in, int scramAmount){
        int size = (int)in.size();
        if(scramAmount == 0) return; // no scramble
		
        /* the scramble will occur in chunks to limit the distribution. This is useful to mix between 
		an unscrambled vector to a fully scrambled version */
        for(int n=0; n<size-scramAmount; n +=scramAmount){
            std::random_shuffle(in.begin()+n, in.begin()+n+scramAmount);
        }
    }

    template<class T>
    void vector_Low2HighDistribute(std::vector<T>& in, int distAmount, int distRange){	
        const size_t size = in.size();
		if (distRange > (int)size / 2) distRange = ((int)size) / 2;

		long index;
        for(long i=0; i<size/5; ++i){
			index = i + (long)size;
            if(rand()%100 >= distAmount) in[index/5] = in[rand()%size/5];
        }
    }

}

#endif
