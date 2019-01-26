#ifndef __include_libs__
#define __include_libs__
#include <cstring>
#include <complex>
#include "kissfft.hh"
#include <cmath>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#endif

class Phasor {
    private:
        int m_sRate;
        float m_Freq;
        double m_increment;
        double m_Phase;
        void i_calcinc(){ m_increment = double(m_Freq)/double(m_sRate);}

    public:
        Phasor();
        Phasor(float freq, int sRate);
        Phasor(float freq, int sRate, float init_Phase);

        void tick();
        void set_freq(float freq);
        void set_rate(int rate);
        void set_phase(float phase){m_Phase = double(phase);}
        double get_phase(){return m_Phase;}

};

template<class T>
class Table{
    private:
        std::vector<T> m_Table;
        int m_Size;
        int m_interp;
        int m_wrap;
        int m_shape;
    public:
        Table(int size);
        Table(int size, int type);
        Table(int size, int type, int interp);
        Table(int size, int type, int interp, int wrap);
        void init_shape(int type);
        void fill_zero();
        void fill_sine();
        void fill_cosine();
        void fill_halfsine();
        void fill_halfcos();
        void fill_triangle(float curve = 0);
        void fill_saw(float curve = 0);
        void fill_square(float curve = 0);
        void fill_han();
        void fill_line(float exp = 0, float scale = 1.f, float offset = 0.f, bool invert = false);
        int get_size(){return m_Size;}
        T get_value(double index);
};

