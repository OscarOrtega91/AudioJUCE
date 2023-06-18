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

DelayLine::~DelayLine() {
    //std::cout<< "Destructor Delay Line" << std::endl;

}
