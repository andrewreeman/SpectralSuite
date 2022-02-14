#include "utilities.h"

namespace utilities {
	int ms2Samps(int ms, int sRate) {
		return ms * (sRate / 1000);
	}

	int wrap(int index, int limit) {
		if (index < 0) {
			return index + limit;
		}
		else {
			return index % limit;
		}
	}

    void car2Pol(std::vector<Cpx> cpxIn, PolarVector& polOut, int size){
        for(int i=0; i<size; ++i){
            polOut[i].m_mag = abs(cpxIn[i]);
            polOut[i].m_phase = arg(cpxIn[i]);
        }
    }
    
    void pol2Car(const PolarVector& polarIn, std::vector<Cpx>& cpxOut, int size){
        const long invertedindexMax = (size * 2) - 1;

        float mag;
        float phase;
        long invertedIndex;

        for(int i=0; i<size; ++i){
            mag = polarIn[i].m_mag;
            phase = polarIn[i].m_phase;
            cpxOut[i] = std::polar(mag, phase);
            
            //and perform the reflection here.
            invertedIndex = invertedindexMax - (long)i;
            cpxOut[invertedIndex] = 0.f;
        }
    }
    
    Polar<FftDecimal> interp_Polar(const Polar<FftDecimal>* v, int lim, float i, bool wrap = false) {
        if (wrap){
          while( (int)i > lim) i -= float(lim);
        }

        float t;
        float r = modff(i, &t);
        int n = int(t);
        Polar<FftDecimal> polOut;
        if ( n< lim-1){
            polOut.m_mag = (1.0f-r)*v[n].m_mag + (r)*v[n+1].m_mag;
            polOut.m_phase = (1.0f-r)*v[n].m_phase + (r)*v[n+1].m_phase;
            return polOut;
        }
        polOut.m_mag = 0.f;
        polOut.m_phase = 0.f;
        return polOut;
    }

    void emptyPolar(PolarVector& inOutPol) {
        Polar<FftDecimal> empty(0.f, 0.f);
        std::fill(inOutPol.begin(), inOutPol.end(), empty);
    }
}
