#ifndef __uscontrols__
#include "uscontrols.h"
#endif

const float GAIN_RANGE = 4.f;

bool NChannelModifier::m_isOpen = false;
bool NChannelModifierXY::m_isOpen = false;

bool dBLabel(float value, std::string& string, CParamDisplay* paramDisplay) {
    if(value == 0.f){
		string.append("-inf");        
    }
    else{
		std::ostringstream stream;
		stream << value << "dB";
		string.append(stream.str());		        
    }

	return true;
};

bool HzLabel(float value, std::string& string, CParamDisplay* paramDisplay) {
	std::ostringstream stream;
	stream.precision(2);
	stream << value << "Hz";
	string.append(stream.str());
	return true;
};

//void MsLabel(float value, char* string){ sprintf(string, "%.2f Ms", value*2000.f); }

bool ChnCtlLabel(float value, std::string& string, CParamDisplay* paramDisplay) {
    value *= 100.f;
    //std::string* pString = (std::string*)chnNumber ;
	std::string* pString = new std::string("FIX ME");
    const char* pChar = (*pString).c_str();

	std::ostringstream stream;
	stream.precision(1);		
	stream << value << "%";
	string.append(stream.str());    
	return true;
}

float getGain(float value, bool normState){
    // Converts the gain from the effect to db values.
    if(value == 0.f){
        return 0.f;
    }
    else{
            if(normState){
                return (10.f * logf(value+0.0001));
            }
            else{
                double outval = 10.0 * logf(value*GAIN_RANGE);
                // this protects again logf(1) == 0 == "-inf" on init display ...
                return outval ? outval : 0.0001;
            }
    }
}

CXySlider::CXySlider (const CRect& size, IControlListener* listener, long tag, std::vector<long> iMinPos,
    std::vector<long> iMaxPos, CBitmap* handle, CBitmap* background,CPoint & offset)
    //in the xySlider, style will set the zero point. to Left|kBottom will set the zero point at the bottom left. kRight|kTop will set at top right.
    : CSlider(size, listener, tag, iMinPos[0], iMaxPos[0], handle, background, offset, style = kLeft|kBottom), values(2),  minLims(2), maxLims(2),
     minPos(2), vmins(2), vmax(2), rangeHandles(2), oldValues(2), defaultValues(2), deltas(2)
{
    vmax[0] = 1.f;
    vmax[1] = 1.f;
    values[0]=0.5;
    values[1]=0.5;
    minPos[0] = iMinPos[0] - size.top;
    rangeHandles[0] = iMaxPos[0] - iMinPos[0];
    minPos[1] = iMinPos[1] - size.left;
    rangeHandles[1] = iMaxPos[1] - iMinPos[1];
}

void CXySlider::bounceValue(){
    // Should really be called "clip values"
	if (values[0] > vmax[0])
		values[0] = vmax[0];
	else if (values[0] < vmins[0])
		values[0] = vmins[0];
    if (values[1] > vmax[1])
		values[1] = vmax[1];
	else if (values[1] < vmins[1])
		values[1] = vmins[1];
}

bool CXySlider::checkDefaultValue (long button){
    if (button == (kControl|kLButton)){
        // Begin edit will tell the host to begin recording for automation
        beginEdit();
		values[0] = getDefaultValues(0) ;
		values[1] = getDefaultValues(1);
		if (isDirty() && listener)
        listener->valueChanged (this);
        endEdit();
		return true;
	}
	return false;
}

 bool CXySlider::isDirty(){
     // If the values are different from the old values then the gui needs updating
    if(oldValues[0] != values[0] || oldValues[1] != values[1] || CView::isDirty() )
        return true;
    return false;
}


void CXySlider::setDirty(const bool val){
    // if setDirty is false then assign the current values to oldValues.
    CView::setDirty (val);
	if (val){
		if (values[0] != -1.f)
			oldValues[0] = -1.f;
		else
			oldValues[0] = 0.f;
        if (values[1] != -1.f)
            oldValues[1] = -1.f;
        else
            oldValues[1] = 0.f;
	}
	else{
		oldValues[0] = values[0];
		oldValues[1] = values[1];
	}
}

void CXySlider::setOffsetHandle (CPoint &val){
    // Sets the minimum and maximum positions for the handle
	offsetHandle = val;
	
    minLims[0] = offsetHandle.x + minPos[0];
    maxLims[0] = minLims[0]+ rangeHandles[0] + widthOfSlider;
    minLims[1] = offsetHandle.y + minPos[1];
    maxLims[1] = minLims[1] + rangeHandles[1] + heightOfSlider;
}

void CXySlider::draw (CDrawContext *pContext){


	CDrawContext* drawContext =  pContext;
	#if 0
	if (pOScreen && bTransparencyEnabled)
		pOScreen->copyTo (pContext, size);
	#endif

    // These will be used as internal holders for the xy values.
	float fValueX;
	float fValueY;

    // If the kRight flag is set then invert the x values. This will place the zero point to the right.
	if(style&kRight)
        fValueX = 1.f-values[0];
    else
        fValueX = values[0];
    if(style&kBottom)
        fValueY = 1.f-values[1];
    else
        fValueY = values[1];

    // Redraw background
    CRect rect (0, 0, widthControl, heightControl);
	CRect size = getViewSize();	
	CBitmap* pBackground = getBackground();
	bool bTransparencyEnabled = getTransparency();

	rect.offset (size.left, size.top);
    if (pBackground){
		if (bTransparencyEnabled)
			pBackground->draw(drawContext, rect, offset, 0.0);			
		else
			pBackground->draw (drawContext, rect, offset);
	}

    // Redraw handle
    CRect rectNew;

    rectNew.left = offsetHandle.x + (int)(fValueX * rangeHandles[0]);
    rectNew.right = rectNew.left + widthOfSlider;
    rectNew.top = offsetHandle.y + (int)(fValueY * rangeHandles[1]);
    rectNew.bottom = rectNew.top + heightOfSlider;
	rectNew.offset (size.left, size.top);
	if (pHandle){
		if (bDrawTransparentEnabled)
			pHandle->draw(drawContext, rectNew, CPoint(0, 0), 0.0);			
        else
			pHandle->draw (drawContext, rectNew);
	}
    //after drawning set the Dirty value to false. This will assign the current value to the oldvalues. The host will call the isDirty() function which will check if the oldValues are different
    // from the current values. If so then I assume draw is called or some screen update function.
	setDirty (false);
}

CMouseEventResult CXySlider::onMouseMoved (CPoint& where, const long& button){

	CRect size = getViewSize();
    	// check if default value wanted
	if (checkDefaultValue (button)) return kMouseEventNotHandled;
    if (!(button & kLButton)) return kMouseEventNotHandled;
    //deltas is a 2 element vector to hold the delta values of x and y.
    deltas[0] = size.left + offsetHandle.x;
    deltas[1] = size.top + offsetHandle.y;
    // Moving the handles centre to the mouse position.
    deltas[0] += widthOfSlider /2 - 1;
    deltas[1] += heightOfSlider / 2 - 1;

	beginEdit ();
    //Modify the values according the mouse position.
    values[0] = (float)(where.y - deltas[0]) / (float)rangeHandles[0];
    values[1] = (float)(where.x - deltas[1]) / (float)rangeHandles[1];
    // check against the style flag.
    if(style & kRight) values[0] = 1.f-values[0];
    else values[0] = values[0];
    if(style & kBottom) values[1] = 1.f-values[1];
    else values[1] = values[1];

    bounceValue ();
    //check if there is a listener (should always be true) and if the dirty flag has been set. If so then tell the editor the value has been modified.
    // The editor will then set the dirty flag to true which will cause a screen redraw only if the new values are different from the old values.
    if (isDirty () && listener) listener->valueChanged (this);
	endEdit ();
	return kMouseEventHandled;
}

NChannelModifier::NChannelModifier(CRect size, CFrame* parentFrame, int numChannels, int enumOffset,
    CBitmap* knobBack, CBitmap* closeHand, CBitmap* title, IControlListener* listener)
    : CViewContainer(getViewSize()), m_numChannels(numChannels), m_enumOffset(enumOffset), m_parentFrame(parentFrame)
{
CColor colour;
    colour.red = 150;

    colour.green = 174;
    colour.blue= 182;
    float widthOffset = knobBack->getWidth()/1.5;
    CRect rect(0, 0, 25, 25);

    setBackgroundColor(colour);
    rect.offset(1, 1);
    m_closeHandle = new CView(rect);
    m_closeHandle->setBackground(closeHand);
    addView(m_closeHandle);

    rect(0, 0, title->getWidth(), title->getHeight());
    rect.offset(25, 25);
    m_title = new CView(rect);
    m_title->setBackground(title);
    addView(m_title);

    rect.setWidth(100);
    rect.offset(10, -25);
    m_textTitle = new CTextLabel(rect);
    m_textTitle->setFont(kNormalFontVeryBig);
    m_textTitle->setStyle(kBoldFace);
    m_textTitle->setBackColor(colour);
    m_textTitle->setFontColor(kBlackCColor);
    m_textTitle->setFrameColor(colour);
    m_textTitle->setHoriAlign(kLeftText);
    addView(m_textTitle);

    mV_labels.resize(m_numChannels, NULL);
    mV_Ctls.resize(m_numChannels, NULL);
    m_ChannelNames.resize(m_numChannels, "");

    makeKnobs(listener, knobBack, widthOffset);
    makeLabels(widthOffset);
    m_isOpen = false;
}

/*
NChannelModifier::~NChannelModifier(){
    for(int i=0; i<m_numChannels; ++i){
        if(mV_Ctls[i]){
            delete mV_Ctls[i];
            mV_Ctls[i] = NULL;
        }
        if(mV_labels[i]){
             delete mV_labels[i];
            mV_labels[i] = NULL;
        }
    }
    if(m_textTitle){
         delete m_textTitle;
         m_textTitle = NULL;
    }
    if(m_title){
         delete m_title;
         m_title = NULL;
    }
    if(m_closeHandle){
         delete m_closeHandle;
         m_closeHandle = NULL;
    }
} */

void NChannelModifier::makeKnobs(IControlListener* listener, CBitmap* knobBack, float widthOffset){
    CRect backSize(0, 0, knobBack->getWidth(), knobBack->getWidth());
    CPoint point(0, 0);

    backSize.offset(widthOffset, knobBack->getWidth());
   for(int i=0; i<m_numChannels; ++i){
        mV_Ctls[i] = new CAnimKnob(backSize, listener, NULL, knobBack, point);
        backSize.offset(knobBack->getWidth() + widthOffset, 0);
        addView(mV_Ctls[i]);
    }
}

void NChannelModifier::makeLabels(float widthOffset){
    CPoint knobPoint;
    int width = 95;
    int height = 15;
    int tinyWidthOffset = 30;
    int yOffset = 30;
    std::string* pLabel;
    std::stringstream stream;

    CColor colour;
    colour.red = (214);
    colour.green = (214);
    colour.blue = (214);

    CColor txtColour;
    txtColour.red = (0);
    txtColour.green = (0);
    txtColour.blue = (0);

    mV_Ctls[0]->localToFrame(knobPoint);
    CRect rect(0, 0, width, height);
    rect.offset(knobPoint.x-width+tinyWidthOffset, knobPoint.y+height+yOffset);
    for(int i=0; i<m_numChannels; ++i){
        mV_labels[i] = new CParamDisplay(rect);
        rect.offset(mV_Ctls[0]->getWidth() + widthOffset, 0);
        stream << "Chn " << (i+1) << ": ";
        m_ChannelNames[i] = stream.str();
        stream.str(std::string()); //clear stream by filling with an empty string

        pLabel = &m_ChannelNames[i];
        //mV_labels[i]->setValueToStringFunction(ChnCtlLabel, (void*)pLabel); //cast to void which will be recast back to a string		

		mV_labels[i]->setValueToStringFunction2(ChnCtlLabel); //cast to void which will be recast back to a string
        mV_labels[i]->setBackColor(colour);
        mV_labels[i]->setFontColor(txtColour);
        //mV_labels[i]->setTxtFace(kBoldFace);
        mV_labels[i]->setStyle(kBoldFace);
        addView(mV_labels[i]);
    }
}

void NChannelModifier::setTag(long tag){
    /* The paramater order is: primary channel modifiable params first. Then channel 1 params. Then channel 2 params..etc
    then the non channel modifiable params */
    for(int i=0; i<m_numChannels; ++i){
        int paramIndexOffset = m_enumOffset * (i+1);
        mV_Ctls[i]->setTag(tag+paramIndexOffset);
    }
}

void NChannelModifier::addToFrame(COnOffButton* editorButton){
    // Pass a pointer to the button that opened the window. This button is now currently disabled. Re-enable when removed from frame.
    m_editorButton = editorButton;
    m_parentFrame->addView(this);
    m_isOpen = true;
}

void NChannelModifier::removeFromFrame(){
    m_editorButton->setMouseEnabled(true);
    m_editorButton->setValue(0);
    m_editorButton = NULL; //reset to null
    m_isOpen = false;
    bounceToParent(); // Pass channel modifier values to be stored in the paramater control.
    m_parentFrame->removeView(this, 0);
    m_parentFrame->setDirty();
}

void NChannelModifier::setChanValue(int index, float value){
    mV_Ctls[index]->setValue(value);
    mV_labels[index]->setValue(value);
}

float NChannelModifier::getChanValue(int index){
    if(mV_Ctls[index]) return mV_Ctls[index]->getValue();
    else return  0.f;
}

void NChannelModifier::bounceToParent(){
    // Pass values from NChannelModifier to the parent.
    for(int i=0; i<m_numChannels; ++i){
        m_pvParentVals->at(i)= getChanValue(i);
    }
}

bool NChannelModifier::isOnCloseHandle(CPoint where){
    CPoint mousePoint = where;
    frameToLocal(mousePoint);
    return m_closeHandle->hitTest(mousePoint);
}

CMouseEventResult NChannelModifier::onMouseDown(CPoint &where, const CButtonState& buttons) {
	CRect size = getViewSize();
   if( isOnCloseHandle(where) && (buttons & kLButton) ){
        removeFromFrame();
		return kMouseEventHandled;
   }
   	// convert to relativ pos
	CPoint where2 (where); //Steinbergs naming. Not mine...
	where2.offset (-size.left, -size.top);

	if (buttons == -1)

		//buttons = pContext->getMouseButtons ();

		return kMouseEventNotHandled;
	/*CView *pSv = 
	while (pSv)
	{
		CView *pV = pSv->pView;
		if (pV && pV->getMouseEnabled () && pV->hitTest (where2, buttons))
		{
			pV->mouse (pContext, where2, buttons);
			break;
		}
		pSv = pSv->pPrevious;
	}*/
}

NChannelModifierXY::NChannelModifierXY(CRect RectSize, IControlListener* listener, CFrame* pParentFrame, int numChannels, CBitmap* knobBack,
    CBitmap* closeHandle, CBitmap* title, int enumOffset)
    : CViewContainer(RectSize), m_numChannels(numChannels), m_enumOffset(enumOffset), m_parentFrame(pParentFrame)
{

    CColor colour;
    colour.red = 150;
    colour.green = 174;
    colour.blue= 182;
    float widthOffset = knobBack->getWidth()/1.5;
    CRect rect(0, 0, 25, 25);
    CRect rect2;

    setBackgroundColor(colour);

    rect.offset(1, 1);
    m_closeHandle = new CView(rect);
    m_closeHandle->setBackground(closeHandle);
    addView(m_closeHandle);

    rect(0, 0, title->getWidth(), title->getHeight());
    rect.offset(80, 30);

    m_title_X = new CView(rect);
    rect2 = rect;
    rect2.offset(0, 112);
    m_title_Y = new CView(rect2);
    m_title_X->setBackground(title);
    m_title_Y->setBackground(title);
    addView(m_title_X);
    addView(m_title_Y);

    rect.setWidth(70);
    rect.offset(-65, 0);
    m_vP_Titles.resize(NCM_Y+1, NULL);

    for(int i=0; i<=NCM_Y; ++i){
        m_vP_Titles[i] = new CTextLabel(rect);
        m_vP_Titles[i]->setFont(kNormalFontVeryBig);
        //m_vP_Titles[i]->setTxtFace(kBoldFace);
        m_vP_Titles[i]->setStyle(kBoldFace);
        m_vP_Titles[i]->setBackColor(colour);
        m_vP_Titles[i]->setFontColor(kBlackCColor);
        m_vP_Titles[i]->setFrameColor(colour);
        m_vP_Titles[i]->setHoriAlign(kLeftText);
        addView(m_vP_Titles[i]);
        rect.setWidth(75);
        rect.offset(0, 112);
    }

    mV_labels.resize(m_numChannels*(NCM_Y+1), NULL);
    mV_Ctls.resize(m_numChannels*(NCM_Y+1), NULL);
    m_ChannelNames.resize(m_numChannels, "");

    makeKnobs(listener, knobBack, widthOffset);
    makeLabels(widthOffset);
    m_isOpen = false;
}



void NChannelModifierXY::setChanValue(bool b_XorY, int chan, float value){
    int XorY_Offset = b_XorY * m_numChannels;
    mV_Ctls[chan+XorY_Offset]->setValue(value);
    mV_labels[chan+XorY_Offset]->setValue(value);
}

float NChannelModifierXY::getChanValue(bool b_XorY, int index){
    int XorY_Offset = b_XorY * m_numChannels;
    return mV_Ctls[index+XorY_Offset]->getValue();
}

void NChannelModifierXY::addToFrame(COnOffButton* handleButton){
    m_parentFrame->addView(this);
    m_editorButton = handleButton;
    m_isOpen = true;
};

void NChannelModifierXY::removeFromFrame(){
    m_editorButton->setMouseEnabled(true);
    m_editorButton->setValue(0);
    m_editorButton = NULL;
    m_parentFrame->removeView(this, 0);
    m_parentFrame->setDirty();
    m_isOpen = false;
};

void NChannelModifierXY::makeKnobs(IControlListener* listener, CBitmap* knobBack, float widthOffset){
    int yOffset = 5;
    int yOffset_2 = 35;
    CPoint point(0, 0);
    CRect backSizeX(0, 0, knobBack->getWidth(), knobBack->getWidth());
    CRect backSizeY(backSizeX);

    backSizeX.offset(widthOffset, knobBack->getWidth() - yOffset);
    backSizeY.offset(widthOffset, knobBack->getWidth()*2 + yOffset);
    backSizeY.offset(0, yOffset_2);

   for(int i=0; i<m_numChannels; ++i){
        mV_Ctls[i] = new CAnimKnob(backSizeX, listener, NULL, knobBack, point);
        mV_Ctls[i+m_numChannels] = new CAnimKnob(backSizeY, listener, NULL, knobBack, point);
        backSizeX.offset(knobBack->getWidth() + widthOffset, 0);
        backSizeY.offset(knobBack->getWidth() + widthOffset, 0);
        addView(mV_Ctls[i]);
        addView(mV_Ctls[i+m_numChannels]);
    }
};

void NChannelModifierXY::setTag(bool b_XorY, long tag){
    int XorY_Offset = b_XorY * m_numChannels;
    for(int i=0; i<m_numChannels; ++i){
        int paramIndexOffset = m_enumOffset * (i+1);
        mV_Ctls[i+XorY_Offset]->setTag(tag+paramIndexOffset);
    }
}

void NChannelModifierXY::bounceToParent(){
    for(int i=0; i<m_numChannels; ++i){
        (*m_vP_XY_Vals)[NCM_X][i] = getChanValue(NCM_X, i);
        (*m_vP_XY_Vals)[NCM_Y][i] = getChanValue(NCM_Y, i);
    }
}

bool NChannelModifierXY::isOnCloseHandle(CPoint where){
    CPoint mousePoint = where;
    frameToLocal(mousePoint);
    return m_closeHandle->hitTest(mousePoint);
};

void NChannelModifierXY::makeLabels(float widthOffset){
    CPoint knobPoint;
    int width = 95;
    int height = 15;
    int tinyWidthOffset = 30;
    int yOffset = 60;
    int yOffset_2 = 45;

    CRect rect_1(0, 0, width, height);
    CRect rect_2(rect_1);
    std::string* pLabel;
    std::stringstream stream;

    CColor colour;
    colour.red = 214;
    colour.green = 214;
    colour.blue = 214;

    CColor txtColour;
    txtColour.red = 0;
    txtColour.green = 0;
    txtColour.blue = 0;

    mV_Ctls[0]->localToFrame(knobPoint); // Convert from local co-ordinates to frame co-ordinates
    rect_1.offset(knobPoint.x-width+tinyWidthOffset, knobPoint.y+height+yOffset);
    rect_2.offset(knobPoint.x-width+tinyWidthOffset, knobPoint.y+height+yOffset);
    rect_2.offset(0, mV_Ctls[0]->getHeight() + yOffset_2);

    for(int i=0; i<m_numChannels; ++i){
        mV_labels[i] = new CParamDisplay(rect_1);
        mV_labels[i+m_numChannels] = new CParamDisplay(rect_2);
        rect_1.offset(mV_Ctls[0]->getWidth() + widthOffset, 0);
        rect_2.offset(mV_Ctls[0]->getWidth() + widthOffset, 0);

        stream << "Chn " << (i+1) << ": ";
        m_ChannelNames[i] = stream.str();
        stream.str(std::string());
        pLabel = &m_ChannelNames[i];
        mV_labels[i]->setValueToStringFunction2(ChnCtlLabel);
        mV_labels[i+m_numChannels]->setValueToStringFunction2(ChnCtlLabel);
        mV_labels[i]->setFontColor(txtColour);
        mV_labels[i+m_numChannels]->setFontColor(txtColour);
        mV_labels[i]->setBackColor(colour);
        mV_labels[i+m_numChannels]->setBackColor(colour);
//        mV_labels[i]->setTxtFace(kBoldFace);
        mV_labels[i]->setStyle(kBoldFace);
//        mV_labels[i+m_numChannels]->setTxtFace(kBoldFace);
        mV_labels[i+m_numChannels]->setStyle(kBoldFace);
        addView(mV_labels[i]);
        addView(mV_labels[i+m_numChannels]);
    }
}


CMouseEventResult NChannelModifierXY::onMouseDown(CPoint &where, const CButtonState& buttons) {
    int height = getHeight();
    int width = getWidth();
	CRect size = getViewSize();

   if( isOnCloseHandle(where) && (buttons & kLButton) ){
        bounceToParent();
        removeFromFrame();
   }

	CPoint where2 (where);
	where2.offset (-size.left, -size.top);	
	return kMouseEventHandled;
	/*
	CCView *pSv = pLastView;
	while (pSv)
	{
		CView *pV = pSv->pView;
		if (pV && pV->getMouseEnabled () && pV->hitTest (where2, buttons))
		{
			pV->mouse (pContext, where2, buttons);
			break;
		}
		pSv = pSv->pPrevious;
	}*/
};

Stereo_CSlider::Stereo_CSlider(const CRect &size, IControlListener *listener, long tag, long iMinPos,
    long    iMaxPos,  CBitmap *handle, CBitmap *background, CPoint  &offset,  NChannelModifier* channelModel,
    const long style)
   : CSlider(size, listener, tag, iMinPos, iMaxPos, handle, background, offset, style), m_Model(NULL), m_name(""){
        setNCM(channelModel);
    }

void Stereo_CSlider::setNCM(NChannelModifier* channelModel){
    m_Model = channelModel;
    m_stereoValues.resize(m_Model->getNumChans(), 1.f);
}

void Stereo_CSlider::setNCM_Values(){
    for(int i=0; i<m_Model->getNumChans(); ++i){
        m_Model->setChanValue(i, m_stereoValues[i]);
        //m_Model->setScaleRange(m_minScale, m_maxScale);
        //m_Model->setAddRange(m_minAdd, m_maxAdd);

    }
}

void Stereo_CSlider::setNCM_Value(int modelCtl_Index, float value){ m_Model->setChanValue(modelCtl_Index, value);}

float Stereo_CSlider::getNCM_Value(int modelCtl_Index)const{
    if(m_Model) return m_Model->getChanValue(modelCtl_Index);
    else return 0.f;
}

void Stereo_CSlider::openNCM(COnOffButton* editorButton){
        setNCM_Values();
        m_Model->setParentValueVector( getValuesPointer() ); // give the NCM the value buffer to fill when NCM window is closed
        m_Model->setTag( this->getTag() );// This will set the NCM to send to THIS parameter
        setNCM_Title();
        m_Model->addToFrame(editorButton);
}

Stereo_XySlider::Stereo_XySlider(const CRect &size, IControlListener *listener, long tagX, long tagY, std::vector<long> iMinPos,
    std::vector<long> iMaxPos,  CBitmap *handle, CBitmap *background, CPoint  &offset,  NChannelModifierXY* channelModel )
   : CXySlider(size, listener, tag, iMinPos, iMaxPos, handle, background, offset), m_Model(NULL), m_tagX(tagX), m_tagY(tagY)
{
        m_vpNames.resize(NCM_Y+1);
        m_vXYstereoValues.resize(NCM_Y+1); //x and y paramaters
        setNCM(channelModel);
};

void Stereo_XySlider::setNCM(NChannelModifierXY* controlModel){
    m_Model = controlModel;
    m_vXYstereoValues[0].resize(m_Model->getNumChans(), 1.f);
    m_vXYstereoValues[1].resize(m_Model->getNumChans(), 1.f);
};

void Stereo_XySlider::setNCM_Values(){
    int numChans = m_Model->getNumChans();
    for(int i=0; i<numChans; ++i){
        m_Model->setChanValue(NCM_X, i, m_vXYstereoValues[NCM_X][i]);
        m_Model->setChanValue(NCM_Y, i, m_vXYstereoValues[NCM_Y][i]);
    }
};

void Stereo_XySlider::setNCM_Titles(){
  m_Model->setParamTitle(NCM_X, m_vpNames[NCM_X]);
  m_Model->setParamTitle(NCM_Y, m_vpNames[NCM_Y]);
}

long Stereo_XySlider::getTag(bool b_XorY)const{
    if(b_XorY == NCM_X){
        return m_tagX;
    }
    else{
        return m_tagY;
    }
}

void Stereo_XySlider::setNCM_Value(bool b_XorY, int modelCtl_Index, float value){
    m_Model->setChanValue(b_XorY, modelCtl_Index, value);
}

void Stereo_XySlider::openNCM(COnOffButton* handleButton){
    setNCM_Values();
    m_Model->setParentValueVector( getValuesPointer() ); // give the NCM the value buffer to fill when NCM window is closed
    m_Model->setTag(NCM_X, getTag(NCM_X));
    m_Model->setTag(NCM_Y, getTag(NCM_Y));
    setNCM_Titles();
    m_Model->addToFrame(handleButton);
}

