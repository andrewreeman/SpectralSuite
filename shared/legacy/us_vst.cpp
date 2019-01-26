#include"us_vst.h"



void Line::trigger(){
    if(m_bFin){
        m_Pos = 0.f;
        m_bFin = false;
    }
}

void Line::tick(){

    if(m_bFin) return;

    m_Pos  += m_Incr*m_Freq;
    clip(m_Pos);

    if(m_Pos == 1.f){
      m_bFin = true;
    }
}

void Line::clip(float& pos){
    if(pos>1.f) pos = 1.f;
}

float Line::getPos()const{
    if(m_bFin) return 1.f;
    else return m_Pos;
}
