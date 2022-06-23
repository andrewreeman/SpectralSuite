#include "wavetable.h"
#include "utilities.h"

//----Phasor----
Phasor::Phasor(): m_sRate(44100), m_Freq(1.f), m_Phase(0.0f)
{
     i_calcinc();
 }

 Phasor::Phasor(float freq, int sRate): m_sRate(sRate), m_Freq(freq), m_Phase(0.f)
{
     i_calcinc();
}

Phasor::Phasor(float freq, int sRate, float init_Phase): m_sRate(sRate), m_Freq(freq),
    m_Phase(double(init_Phase))
{
    i_calcinc();
}

void Phasor::tick(){
    m_Phase += m_increment;
    while (m_Phase >= 1.0) m_Phase -= 1.0;
    while (m_Phase <=0.0) m_Phase += 1.0;
}

void Phasor::set_freq(float freq){
    m_Freq = freq;
    i_calcinc();
}

void Phasor::set_rate(int rate){
    m_sRate = rate;
    i_calcinc();
}

//----Table----
template<typename T>
Table<T>::Table(int size) : m_Size(size), m_Table(size+2), m_interp(1), m_wrap(1),  m_shape(0)
{
    init_shape(0);
}

template<typename T>
Table<T>::Table(int size, int type) : m_Size(size), m_Table(size+2), m_interp(1),  m_wrap(1), m_shape(type)
{
    init_shape(type);
}

template<typename T>
Table<T>::Table(int size, int type, int interp) : m_Size(size), m_Table(size+2), m_interp(interp), m_wrap(1), m_shape(type)
{
    init_shape(type);
}

template<typename T>
Table<T>::Table(int size, int type, int interp, int wrap) : m_Size(size+2),  m_Table(size+2), m_interp(interp), m_wrap(wrap), m_shape(type)
{
    init_shape(type);
}

template <typename T>
void Table<T>::init_shape(int type){
    switch(type){
        case 0:
            this->fill_zero();
            break;
        case 1:
            this->fill_sine();
            break;
        case 2:
            this->fill_cosine();
            break;
        case 3:
            this->fill_halfsine();
            break;
        case 4:
            this->fill_halfcos();
            break;
        case 5:
            this->fill_saw();
            break;
        case 6:
            this->fill_triangle();
            break;
        case 7:
            this->fill_line();
        default:
            this->fill_zero();
            break;
    };
};

template<typename T>
void Table<T>::fill_zero(){
 for(int n=0; n<=m_Size; ++n) m_Table[n] = T(0.0); //including the last point
}

template<typename T>
void Table<T>::fill_sine(){

    for(int n=0; n<m_Size; ++n){
        m_Table[n] = sin(TWOPI*n/m_Size);
    }
    m_Table[m_Size] = T(0.0); //the last point will be the guard point
};

template<typename T>
void Table<T>::fill_cosine(){
    for(int n=0;n<m_Size; ++n){
        m_Table[n] = cos(TWOPI*n/m_Size);
    }
    m_Table[m_Size] = T(1.0);
};

template<typename T>
void Table<T>::fill_halfsine(){

    for(int n=0; n<m_Size; ++n){
        m_Table[n] = sin(TWOPI*n*0.5/m_Size);
    }
    m_Table[m_Size] = T(0.0);
};

template<typename T>
void Table<T>::fill_halfcos(){

    for(int n=0; n<m_Size; ++n){
        m_Table[n] = cos(TWOPI*n*0.5/m_Size);
    }
    m_Table[m_Size] = T(-1.0);
};

template<typename T>
void Table<T>::fill_triangle(float curve){

    for(int n=0;n<int(m_Size/2); ++n){
        m_Table[n] = 2.0*T(n)/T(m_Size);
    };

    for(int n=int(m_Size/2); n<m_Size; ++n){
        m_Table[n] = 2.0*(1.0-(T(n)/T(m_Size)));
    };
    m_Table[m_Size] = T(0.0);
};

template<typename T>
void Table<T>::fill_line(float curve, float scale, float offset, bool invert){
  for(int n=0; n<m_Size; ++n){
    curve = pow(n, curve);
    m_Table[n] = (T(n)/T(m_Size)*curve)*scale+offset;
  }
  if(invert){
      for(int n=0; n<m_Size; ++n){
          m_Table[n] = m_Table[m_Size-n];
      }
  }
}

template<typename T>
void Table<T>::fill_saw(float curve){ //saw contains all the harmonics with amplitudes of 1/harmonic.
    curve = 1.0 - curve; //invert
    curve *= 25;
    int max_harmonics = int(curve+1);
    int cur_harmonic = 1;
    //amp factor will be amplitude of each sine
    // scale will be the scaling factor required to bring into the -1, 1 range.
    double amp_fact, scale;

    for (int n=0; n<max_harmonics; ++n){
        amp_fact = 1.0f/double(cur_harmonic);
        //this expression for scale ... works
        scale = 0.5+0.5*(cur_harmonic/max_harmonics);
        for (int i=0;i<m_Size; ++i){
            m_Table[i] += (amp_fact*sin(TWOPI*i*cur_harmonic/double(m_Size)))*scale;
        };
        cur_harmonic += 1;

    };
    m_Table[m_Size] = T(0.0);
};

template<typename T>
T Table<T>::get_value(double _index){
    int index = (int)_index % m_Table.size();
            
    switch(m_interp){
        case 1:
        {
            
            //TODO: will need to add a guard point if linear interp ...
            T vala = m_Table[index];
            
            int nextIndex = index + 1;
            if(nextIndex == m_Table.size()) {
                nextIndex = 0;
            }
            
            T valb = m_Table[nextIndex];
                        
            return utilities::interp_lin<T>(vala, valb, _index);
        } break;
        default:
            return m_Table[int (index)];
            break;
    }
};

template<typename T>
void Table<T>::resize(int newSize) {
    m_Size = newSize;
    m_Table.resize(newSize + 2, T());
    m_interp = 1;
    m_wrap = 1;
    init_shape(m_shape);
}

template class Table<float>;
template class Table<double>;








