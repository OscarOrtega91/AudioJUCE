//
//  DelayLine.cpp
//  Reverb_VST
//
//  Created by Oscar Ortega on 16/12/20.
//

#include "DelayLine.h"

#include <iostream>
#include <cstring>

DelayLine::DelayLine()
{
    rptr = wptr = 0;
    m_maxdelay = 48000; // 1 sec max
    m_delay = 8000; // how many samples delay?
    if(dline)
    {
        delete [] dline;
        dline = NULL;
     }
    dline = new double[m_maxdelay]; // dynamically allocate 48000 samples
    memset(dline,0,m_maxdelay*sizeof(double));
}

//Change Delay Length for sampling
void DelayLine::changeDelayLength(int sample)
{
    rptr = wptr = 0;
    m_maxdelay = (int)sample; // 1 sec max
    if(dline)
    {
        delete [] dline;
        dline = NULL;
     }
    dline = new double[m_maxdelay]; // dynamically allocate 48000 samples
    memset(dline,0,m_maxdelay*sizeof(double));
}

//change delay M_Delay
void DelayLine::changeMDelay(double delay)
{
    double temp=(delay*0.001)*m_maxdelay;

    m_delay=(int)temp;
}



//-----------------------------------------------------
double DelayLine::process(double in)

{
    double out;
    //processing
    //calculate read pointer position
    rptr = wptr - m_delay; // read pointer offset value (0->Fs)
    if (rptr < 0)
    {
        rptr+=m_maxdelay; // if it’s less than 0, wrap it round
    }
    
    //Algorithm for echo
    out= dline[rptr];
    dline[wptr]=in;
    
    //increment write pointer
    wptr++; // add one to the write pointer
    if(wptr>=m_maxdelay)
    {
        wptr = 0; // wrap it back to zero if it’s gone bigger than max delay
    }
    return out;
}

DelayLine::~DelayLine(void) {
    if(dline)
    {
        delete [] dline;
        dline = NULL;
     }
}
