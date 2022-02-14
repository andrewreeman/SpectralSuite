#include "Polar.h"
//#include "Types.h"

template <class T>
Polar<T> Polar<T>::operator*(T mult){
    Polar<T> result;
    result.m_mag = m_mag * mult;
    result.m_phase = m_phase * mult;
    return result;
}

//
//template<class T>
//void car2Pol(std::vector<Cpx> cpxIn, std::vector< Polar<T> >& polOut, int size){
//    for(int i=0; i<size; ++i){
//        polOut[i].m_mag = abs(cpxIn[i]);
//        polOut[i].m_phase = arg(cpxIn[i]);
//    }
//}
//
//template<class T>
//void pol2Car(const std::vector<Polar<T> >& polarIn, std::vector<Cpx>& cpxOut, int size){
//    const long invertedindexMax = (size * 2) - 1;
//
//    float mag;
//    float phase;
//    long invertedIndex;
//
//    for(int i=0; i<size; ++i){
//        mag = polarIn[i].m_mag;
//        phase = polarIn[i].m_phase;
//        cpxOut[i] = std::polar(mag, phase);
//
//        //and perform the reflection here.
//        invertedIndex = invertedindexMax - (long)i;
//        cpxOut[invertedIndex] = 0.f;
//    }
//}
//
//template <class T>
//Polar<T> interp_Polar(const Polar<T>* v, int lim, float i, bool wrap = false){
//    if (wrap){
//      while( (int)i > lim) i -= float(lim);
//    }
//
//    float t;
//    float r = modff(i, &t);
//    int n = int(t);
//    Polar<T> polOut;
//    if ( n< lim-1){
//        polOut.m_mag = (1.0f-r)*v[n].m_mag + (r)*v[n+1].m_mag;
//        polOut.m_phase = (1.0f-r)*v[n].m_phase + (r)*v[n+1].m_phase;
//        return polOut;
//    }
//    polOut.m_mag = 0.f;
//    polOut.m_phase = 0.f;
//    return polOut;
//}
//
//void emptyPolar(PolarVector& inOutPol){
//    Polar<FftDecimal> empty(0.f, 0.f);
//    std::fill(inOutPol.begin(), inOutPol.end(), empty);
//}
