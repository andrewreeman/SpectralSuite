/*
    us_controls.h contains:

     XY Controller for changing two paramaters simultaneously.
     NChannelModiifer (NCM) for varying effect parameters between channels. Used with Sliders and other
     single parameter objects that are designed to communicate to NChannelModifier objects.
     XYNChannelModifier for varying two effect parameters between channels.  Used with XY controllers that are designed
     to communicate to NChannelModifier objects.
     Sliders and XY Controllers that are designed to communicate with NChannelModifier.
 */

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

#ifndef __vstgui__
#include "vstgui/vstgui.h"
#endif

#ifndef __uscontrols__
#define __uscontrols__

enum {
  NCM_X = 0,
  NCM_Y
};

// These functions are used to add a label to the parameter display
bool dBLabel(float value, std::string& string, CParamDisplay* paramDisplay);
bool HzLabel(float value, std::string& string, CParamDisplay* paramDisplay);
//void MsLabel(float  value, char* string);
bool ChnCtlLabel(float value, std::string& string, CParamDisplay* paramDisplay);
float getGain(float value, bool normState);

class CXySlider: public CSlider{
    /*
        CSlider derives from CControl. CxySlider combines the code in the horizontal and vertical sliders
        most of the data members are vectors that hold x and y values
        This class is no longer used in the spectral suite effects. It has been replaced by a child class
        that is designed to accept NChannelModifiers.
    */

    public:
        //from CControl
        void setValue(int ind, float val){values[ind] = val;}
        void setValues(float valx, float valy){values[0] = valx, values[1] = valy;}
        float getValue(int ind)const { return values[ind];}

        void setMins (int ind, float val) {vmins[ind] = val;}
        float getMins (int ind) const {return vmins[ind];}
        void setMax (int ind, float val) {vmax[ind] = val;}
        float getMax (int ind) const {return vmax[ind];}

        void setOldValues (int ind, float val) {oldValues[ind] = val; }
        float getOldValues(int ind) const {return oldValues[ind]; }

        void setDefaultValues (int ind, float val) { defaultValues[ind] = val; }
        float getDefaultValues (int ind) const {return defaultValues[ind]; }

        void bounceValue();
        bool checkDefaultValue(long button);
        bool isDirty(); // if dirty is true then a redraw will be triggered
        void setDirty(const bool val = true);

        //from CSlider
        virtual CMouseEventResult onMouseMoved (CPoint& where, const long& buttons);

        void draw(CDrawContext* pcontext);
        void setOffsetHandle(CPoint& point);

        CXySlider (const CRect &size, IControlListener* listener, long tag, std::vector<long> iMinPos,
            std::vector<long> iMaxPos, CBitmap *handle, CBitmap *background, CPoint  &offset
        );

    protected:

        //the boundry limits/ Using vectors as I will be dealing with X values for the first element and Y value for the second.
        std::vector<long> minLims; // min and max limits for the handle movement
        std::vector<long> maxLims;
        std::vector<long> minPos;
        std::vector<long> rangeHandles; // can set a range instead of setting min and max limits

        std::vector<float> vmins; // min and max limits for the value
        std::vector<float> vmax;
        std::vector<float> values; // current value
        std::vector<float> oldValues; // used to find the delta values
        std::vector<long> deltas;
        std::vector<float> defaultValues;
};


class NChannelModifier : public CViewContainer{
    /*
        An NChannelModifier (NCM) will be linked to an effect parameter, named the parent parameter. The NCM will modify the effect value used for different channels.
        For instance, if it is linked to a delay effect the NCM can set different values of delay for the left and right channels.
        The non-ncm control will control the primary value while the NCM controls will scale this amount for use in different channels.
        For instance, if the delay slider is set to 0.5 and the NCM left is set to "1" and NCM right is set to 0.5 then the left channel delay value will
        be 0.5*1 = 0.5 and the right channel delay value will be 0.5*0.5=0.25.The NCM controls at the moment range from 0 to 1.

        The user will click on a button next to the parameter paramater to open up the NCM window which will contain the controls for modifiying the parameter.
        The user will click on a cross to close the window.

        A single NChannelModifier will be used for each CControl object that use the same number of channels.
        It is inefficient to create a unique NChannelModifier for each CControl object so instead we will create one NChannelModifier "model".
        Each control will simply update the gui values and routing when it is opened.
    */

    public:
        static bool m_isOpen; //is the NCM window open. If so then the parent parameter buttons will be disabled to prevent multiple instances opening.

    public:
        NChannelModifier(CRect pSize, CFrame* pParentFrame, int numChannels, int enumOffset,
                         CBitmap* knobBack, CBitmap* closeHand, CBitmap* title, IControlListener* listener);
        //NChannelModifier(const NChannelModifier &other);
        //NChannelModifier& operator=(const NChannelModifier &other);
        ~NChannelModifier(){};

        void setParamTitle(const char* title){m_textTitle->setText(title);}
        void setLabel(int index, const char* label); // sets the prefix of each paramter display. This will usually be "Ch1"..."Ch2" but can be modified.
        int getNumChans()const{return m_numChannels;}
        void setChanValue( int Ctl_Index, float value);
        float getChanValue(int index);
        void setTag(long tag); // sets which paramater to send valueChanged to.
        void addToFrame(COnOffButton* editorButton);
        void removeFromFrame();
        bool isOpen(){return m_isOpen;}
        void setParentValueVector(std::vector<float>* pv_Values){ m_pvParentVals = pv_Values; }  // the NCM values will be stored in the parent paramater control object.
		CMouseEventResult onMouseDown(CPoint &where, const CButtonState& buttons);
        
        // void setScaleRange(int min, int max);
        // void setAddRange(int min, int max);

    private:

        bool isOnCloseHandle(CPoint where); // is the mouse on the close button of the NCM.
        void bounceToParent(); //  When the NCM closes it will copy the modifier values (for Ch1 and Ch2..etc) to the parent paramter.
        void makeKnobs(IControlListener* listener, CBitmap* knobBack, float widthOffset);
        void makeLabels(float widthOffset);

        int m_numChannels;
        COnOffButton* m_editorButton; // the editor button is the button next to the parent parameter that opened the NCM.
        // When NCM is opened the button will be disabled. When the NCM closes it will re-enable the button
        CFrame* m_parentFrame;
        IControlListener* m_listener;
        CView* m_closeHandle; // Handle to close the NCM when clicked
        CView* m_title;
        CTextLabel* m_textTitle;
        std::vector<float>* m_pvParentVals; // This is a pointer to the vector held in the parent parameter that stores the modifier values.
        std::vector<CParamDisplay*> mV_labels;
        std::vector<CAnimKnob*> mV_Ctls;
        std::vector<std::string> m_ChannelNames;
        int m_enumOffset; // This will route the knobs to the correct channel parameter in the effect.
        //int m_effectParameter;
        // int m_minScale; // scale OR add
        // int m_maxScale;
        // int m_minAdd;
        // int m_maxAdd;
        //bool mb_Relative;  //relative (scaling) or absolute (assigning) values
};



class NChannelModifierXY : public CViewContainer{
    /*
        A version of NChannelModifier that displays and modifies two paramaters. To be used in connection with XY controllers.
    */
    public:
            static bool m_isOpen;
    public:
        NChannelModifierXY(CRect pSize, IControlListener* listener, CFrame* pParentFrame, int numChannels, CBitmap* knobBack, CBitmap* closeHandle,
                        CBitmap* title, int enumOffset);
         ~NChannelModifierXY(){};

        void setLabel(int index, const char* label);
        void makeLabels(float widthOffset);
        int getNumChans()const{return m_numChannels;}
        void removeFromFrame();
        void addToFrame(COnOffButton* handleButton);
        float getChanValue(bool b_XorY, int index);
        void setParamTitle(int param, const char* title){m_vP_Titles[param]->setText(title); }
        //void mouse(CDrawContext *pContext, CPoint &where, long buttons = -1);
		CMouseEventResult onMouseDown(CPoint &where, const CButtonState& buttons);
        void setChanValue(bool b_XorY, int chan, float value);
        void setTag(bool b_XorY, long tag);
        void setParentValueVector(std::vector< std::vector<float> >* pv_Values){ m_vP_XY_Vals = pv_Values; }
        bool isOpen(){return m_isOpen;}
        //void setScaleRange(int min, int max);
        //void setAddRange(int min, int max);

    private:
        void makeKnobs(IControlListener* listener, CBitmap* knobBack, float widthOffset);
        void bounceToParent();
        bool isOnCloseHandle(CPoint where);

        std::vector< std::vector<float> >* m_vP_XY_Vals;
        std::vector<CTextLabel*> m_vP_Titles;
        std::vector<std::string> m_ChannelNames;
        int m_numChannels;
        CFrame* m_parentFrame;
        COnOffButton* m_editorButton;
        CView* m_closeHandle;
        CView* m_title_X;
        CView* m_title_Y;
        std::vector<CParamDisplay*> mV_labels;
        std::vector<CAnimKnob*> mV_Ctls;
        int m_enumOffset;
        //int m_minScale; // scale OR add
        //int m_maxScale;
        //int m_minAdd;
        //int m_maxAdd;
};

class Stereo_CSlider : public CSlider{
    /*
        A slider that communicates to an NCM object. The slider will store the NCM modifier values which are retrieved when the NCM is opened from the
        respective editor button.
        Each slider will contain a pointer to an NCM. When the NCM is opened the slider will set the NCM's gui values and title. This saves there having
        to be a completely separate NCM instance for each control.
    */
    public:
        Stereo_CSlider (const CRect &size, IControlListener *listener, long tag, long iMinPos,
            long iMaxPos, CBitmap *handle, CBitmap *background, CPoint  &offset, NChannelModifier* model,
            const long style = kLeft|kHorizontal );

        void openNCM(COnOffButton* editorButton);
        void setNCM(NChannelModifier* controlModel); // Set the pointer to the NCM. This is the "model" that shall be used for paramater modifier window.
        int getNChannels()const{ m_Model->getNumChans();}
        float getNCM_Value(int modelCtl_Index)const;
        void setName(const char* name){ m_name = name;} // Set the name of the effect.
        std::vector<float>* getValuesPointer(){ return &m_stereoValues;}
        void setNCM_Value(int modelCtl_Index, float value);
        //void setScaleRange(int min, int max);
        //void setAddRange(int min, int max);

    private:
        void setNCM_Title(){m_Model->setParamTitle(m_name);}
        void setNCM_Values();
        //void sendScaleRange()const;
        //void sendAddRange()const;

        NChannelModifier* m_Model;
        std::vector<float> m_stereoValues;
        const char* m_name;
        //int m_minScale;
        //int m_maxScale;
        //int m_minAdd;
        //int m_maxAdd;
};


class Stereo_XySlider : public CXySlider{
    /*
        This is a modification of the XY Slider that will communicate with an NCM and will also take two tags instead of one.
    */
 public:
    Stereo_XySlider (const CRect &size, IControlListener *listener, long tagX, long tagY, std::vector<long> iMinPos,
            std::vector<long> iMaxPos, CBitmap *handle, CBitmap *background, CPoint  &offset, NChannelModifierXY* model);

        void openNCM(COnOffButton* handleButton);
        void setNCM(NChannelModifierXY* controlModel);
        int getNChannels()const{ m_Model->getNumChans();}
        void setNCM_Value(bool b_XorY, int modelCtl_Index, float value);
        float getNCM_Value(bool b_XorY, int modelCtl_Index){return  m_Model->getChanValue(b_XorY, modelCtl_Index);}
        void setName(int param, const char* name){ m_vpNames[param] = name;}
        void setNCM_Titles();
        long getTag(bool b_XorY)const;		
		int32_t getTag()const{ return m_tagX; } // when called by valueChanged
        //void setScaleRange(int min, int max);
        //void setAddRange(int min, int max);

    private:
        std::vector< std::vector<float> > * getValuesPointer(){ return &m_vXYstereoValues;}
        void setNCM_Values();
        //void sendScaleRange()const;
        //void sendAddRange()const;

        long m_tagX;
        long m_tagY;
        std::vector<const char* > m_vpNames;
        NChannelModifierXY* m_Model;
        std::vector< std::vector<float> > m_vXYstereoValues; //holds a vector for x and one for y values
        int m_minScale;
        int m_maxScale;
        int m_minAdd;
        int m_maxAdd;
};
#endif
