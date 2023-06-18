//
//  AllPassReverbDamping.cpp
//  Reverb_VST
//
//  Created by Oscar Ortega on 23/12/20.
//

#include "AllPassReverbDamping.h"

AllPassReverbDamping::AllPassReverbDamping()
{
    //std::cout<< "Constructor AllPassReverb Damping" << std::endl;

    m_alpha = 0.65; // allpass gain coefficient
    //memset(dline,0,m_maxdelay*sizeof(double));
    //Set Variables for Feedback and Damping One Pole Filter
    M_OPFB=0.6; //Damping
    M_OPFF=1-M_OPFB; //FeedForward Coefficient
    M_OPMM=0;// Memory for Damping, One pole Filter
    m_fbGain=0.6;//Feedback Gain
}

AllPassReverbDamping::~AllPassReverbDamping(void) {
    //std::cout<< "Destructor AllPassReverb Damping" << std::endl;

}

//-----------------------------------------------------
double AllPassReverbDamping::process(double in)

{
    double out;
    double Vn;
    double d_out;
    //processing
    //calculate read pointer position
    rptr = wptr - m_delay; // read pointer offset value (0->47999)
    if (rptr < 0)
    {
        rptr+=m_maxdelay; // if it’s less than 0, wrap it round
    }
    
    //Algorithm for echo
    d_out = dline[rptr]; // output from delay line
    Vn = in - d_out*m_alpha; // calculate sample after the first summation (v(n))
    out = Vn*m_alpha + d_out; // calculate the output
    
    dline[wptr] = (M_OPFF *(m_fbGain*Vn)) + M_OPMM; // update the delay line with v(n)
    M_OPMM= dline[wptr] * M_OPFB;
    
    //increment write pointer
    wptr++; // add one to the write pointer
    if(wptr>=m_maxdelay)
    {
        wptr = 0; // wrap it back to zero if it’s gone bigger than max delay
    }
    return out;
}
