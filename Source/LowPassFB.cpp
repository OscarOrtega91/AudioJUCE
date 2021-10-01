//
//  LowPassFB.cpp
//  Reverb_VST
//
//  Created by Oscar Ortega on 21/12/20.
//

#include "LowPassFB.h"
#include <iostream>

LowPassFB::LowPassFB(){
    // Set Pointers
    rptr=0;
    wptr=0;
    //Set Variables for Circular Buffer
    m_maxdelay=48000; //1 sec max
    m_delay=8000;     //Delay for Input
    if(dline){
        delete [] dline;
        dline = NULL;
    }
    dline = new double[m_maxdelay]; // dynamically allocate 48000 samples
    memset(dline,0,m_maxdelay*sizeof(double));
    
    //Set Variables for Feedback and Damping One Pole Filter
    M_OPFB=0.6; //Damping
    M_OPFF=1-M_OPFB; //FeedForward Coefficient
    M_OPMM=0;// Memory for Damping, One pole Filter
    m_fbGain=0.6;//Feedback Gain
}

LowPassFB::~LowPassFB()
{
    if(dline)
    {
        delete [] dline;
        dline = NULL;
     }
}

void LowPassFB::changeDelayLength(int sample){
    rptr = wptr = 0;
    m_maxdelay = sample; // 1 sec max
    if(dline)
    {
        delete [] dline;
        dline = NULL;
     }
    // dynamically allocate Sampling rate
    dline = new double[m_maxdelay];
    memset(dline,0,m_maxdelay*sizeof(double));
}

void LowPassFB::changeMDelay(double delay){
    double temp=(delay*0.001)*m_maxdelay;
    m_delay=(int)temp;
}

double LowPassFB::process(double in){
    double out;
    //processing
    //calculate read pointer position
    rptr = wptr - m_delay; // read pointer offset value (0->47999)
    if (rptr < 0)
    {
        rptr+=m_maxdelay; // if it’s less than 0, wrap it round
    }
    
    //Algorithm for echo
    out= dline[rptr];
    dline[wptr]=in + (M_OPFF *(m_fbGain*out)) + M_OPMM;
    M_OPMM= dline[wptr] * M_OPFB;
    
    
    //increment write pointer
    wptr++; // add one to the write pointer
    if(wptr>=m_maxdelay)
    {
        wptr = 0; // wrap it back to zero if it’s gone bigger than max delay
    }
    return out;
}
