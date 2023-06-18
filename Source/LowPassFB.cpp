//
//  LowPassFB.cpp
//  Reverb_VST
//
//  Created by Oscar Ortega on 21/12/20.
//

#include "LowPassFB.h"
#include <iostream>

LowPassFB::LowPassFB(){
    //std::cout<< "Constructor LowPass FB" << std::endl;

    //Set Variables for Feedback and Damping One Pole Filter
    M_OPFB=0.6; //Damping
    M_OPFF=1-M_OPFB; //FeedForward Coefficient
    M_OPMM=0;// Memory for Damping, One pole Filter
    m_fbGain=0.6;//Feedback Gain
}

LowPassFB::~LowPassFB()
{
    //std::cout<< "Destructor LowPass FB" << std::endl;
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
