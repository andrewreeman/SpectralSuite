#pragma once

#include <complex>

template <class T>
struct Polar{
    T m_mag;
    T m_phase;
    Polar() : m_mag(0), m_phase(0){}

    /* The explicit keyword means that a constructor will be `non-converting'.
       This means that we can only use the constructor for initializing this class.
    So, We cannot do Polar pol1 = 34 which could assign 34 to mag and 0 to phase.
    We can do Polar pol1(43, 0) or Polar pol1 = Polar(34, 0) */
    explicit Polar(const T& mag, const T& phase){
            m_mag = mag;
            m_phase = phase;
    }
    explicit Polar(const std::complex<T>& cmplx){
        m_mag = abs(cmplx);
        m_phase = arg(cmplx);
    }
    Polar<T> operator* (T mult);
    
    
};
