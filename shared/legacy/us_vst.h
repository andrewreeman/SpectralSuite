#ifndef US_VST_H
#define US_VST_H

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

/*-----
The TwoChanParams struct is for holding two equal parameter values.
These values relate to the left and right channels. Using the chanMod function
we can create a difference between the left and right values and, depending upon what these parameters are used for,
enhance the stereo image.

Andrew Reeman - 01/08/2012
-----*/
/*
template <class T>
struct TwoChanParams{
    T left;
    T right;
    T modExponent; // used in the chanMod function
    T exponentOffset; // ""

    TwoChanParams() : left( T(0) ), right( T(0) ), modExponent( T(1) ), exponentOffset( T(1.f) ) {};
    T avgLevel();
    void chanMod(T modifier);
    void operator= (const T other);
    TwoChanParams operator* (const T other);
};

//-----Definitions-----
template <class T>
void TwoChanParams<T>::chanMod(T modifier)
{	// In the vst effects the m_stereoAmt parameter is passed to chanMod
    right = pow(left, modifier*modExponent+exponentOffset);
};

template<class T>
T TwoChanParams<T>::avgLevel(){

	//returns the average of both parameters
    return (this->left+this->right)*T(0.5);
};

template <class T>
TwoChanParams<T> TwoChanParams<T>::operator*(const T other){
    this->left *= other;
    this->right *= other;

    return *this;
};


template <class T>
void TwoChanParams<T>::operator=(const T other){
    left = other;
    right = other;
};
*/
template <class T>
class NChanParamModifer{
    private:
        std::vector<T> mV_modifiers;
        std::vector<T> mV_postModParams;
        T m_primaryInput;
        bool m_addMode;
        T m_min;
        T m_max;
        int m_numChans;
        std::string m_Label;
        std::vector<std::string> mV_chanLabels;

        void clipInput();

    public:
        NChanParamModifer(int numChans);
        NChanParamModifer(int numChans, T min, T max); // set "min" and "max"  both to NULL for no clipping
        NChanParamModifer(int numChans, T min, T max, bool scaleOrAdd);

        void setMode(bool scaleOrAdd){ m_addMode = scaleOrAdd; }
        bool getMode()const{ return m_addMode;}

        void setLabel(std::string name) { m_Label = name;}
        void setLabel(const char* name) { m_Label = name;}
        std::string getLabel()const{return m_Label;}

        void setChanLabel(int chan, std::string label){ mV_chanLabels[chan] = label;}
        void setChanLabel(int chan, const char * label){ mV_chanLabels[chan] = label;}
        std::string getChanLabel(int chan)const{return mV_chanLabels[chan];}

        void setNChans(int nChans);
        int getNChans(){ return m_numChans;}

        void setModifier(int channel, T input){ mV_modifiers[channel] = input; }
        T getModifier(int channel)const{ return mV_modifiers[channel];}

        void setPrimaryInput(T input){ m_primaryInput = input; }
        T getPrimaryInput()const { return m_primaryInput;}

        T getChanValue(int chan);

        void setMin(T min) { m_min = min;}
        T getMin()const{return m_min;}

        void setMax(T max) { m_max = max;}
        T getMax()const{return m_max;}

        //void modify(TwoChanParams<T>* outParams);
};

template <class T>
NChanParamModifer<T>::NChanParamModifer(int numChans): m_numChans(numChans), m_min(NULL), m_max(NULL), m_addMode(false),
    m_primaryInput(0)
{
    mV_modifiers.resize(m_numChans);
    mV_postModParams.resize(m_numChans);
}

template <class T>
NChanParamModifer<T>::NChanParamModifer(int numChans, T min, T max): m_numChans(numChans), m_min(min), m_max(max),
    m_addMode(false), m_primaryInput(0)
{
    mV_modifiers.resize(m_numChans);
    mV_postModParams.resize(m_numChans);
    mV_chanLabels.resize(m_numChans);

}

template <class T>
NChanParamModifer<T>::NChanParamModifer(int numChans, T min, T max, bool m_addMode): m_numChans(numChans), m_min(min),
    m_max(max), m_addMode(m_addMode), m_primaryInput(0)
{
    if(m_addMode){
        mV_modifiers.resize(m_numChans, T(0));
    }
    else{
        mV_modifiers.resize(m_numChans, T(1));
    }
    mV_postModParams.resize(m_numChans, T(0));
}

template <class T>
void NChanParamModifer<T>::clipInput(){
    if(m_min == NULL && m_max == NULL) return;  // no clip if both NULL
    for(int i=0; i<m_numChans; ++i){
        if(mV_postModParams[i] < m_min) mV_postModParams[i] = m_min;
        else if(mV_postModParams[i] > m_max) mV_postModParams[i] = m_max;
    }
}

template <class T>
void NChanParamModifer<T>::setNChans(int newChans){
    m_numChans = newChans;
    mV_modifiers.resize(m_numChans);
    mV_postModParams.resize(m_numChans);
}

template <class T>
T NChanParamModifer<T>::getChanValue(int chan){
    if(m_addMode){
            mV_postModParams[chan] = m_primaryInput + mV_modifiers[chan];

    }
    else{
            mV_postModParams[chan] = m_primaryInput * mV_modifiers[chan];
    }
    clipInput();
    return mV_postModParams[chan];
}

class Line{
    private:
        float m_Pos;
        float m_Incr;
        bool m_bFin;
        float m_Freq;
    public:

        Line(float increment): m_Incr(increment), m_Pos(0.f), m_bFin(true), m_Freq(1.0){}
        ~Line(){};
        void trigger();
        float getPos()const;
        void setIncr(float increment){ m_Incr = increment;}
        void setFreq(float freq){m_Freq = freq;}
        void tick();

    private:
        void clip(float& pos);

};


#endif
