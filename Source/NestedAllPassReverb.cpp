//
//  NestedAllPassReverb.cpp
//  Reverb_VST
//
//  Created by Oscar Ortega on 16/12/20.
//

#include "NestedAllPassReverb.h"
#include <iostream>

NestedAllPassReverb::NestedAllPassReverb()
{
    //std::cout<< "Constructor Nested AllPassReverb" << std::endl;

    m_alpha = 0.65; // allpass gain coefficient
    
    rptrNested=0;
    wptrNested=0;
    m_delayNested=8000;
    m_alphaNested=0.5;
    
    cleanNestedBuffer();
    allocateNestedBuffer();
    

}
NestedAllPassReverb::~NestedAllPassReverb(){
    //std::cout<< "Destructor NEsted AllPassReverb" << std::endl;

    cleanNestedBuffer();
    
}

//Change Lenght Circular Buffer
void NestedAllPassReverb::changeDelayLength(int sample)
{
    
    ReverbModule::changeDelayLength(sample);
    
    cleanNestedBuffer();
    allocateNestedBuffer();
}

void NestedAllPassReverb::changeMDelayNested(double delay){
    // Change Value for Nested AP m_Delay
    // Variable for delay should be send in secs
    // Function to change it to a sample Number
    double temp=(delay*0.001)*m_maxdelay;
    m_delayNested=(int)temp;
}

//-----------------------------------------------------
double NestedAllPassReverb::process(double in)
{
    double out;
    double Vn;
    double Tn;
    double d_out;
    double d_outNested;
    double outNested;

    //processing
    //calculate read pointer position Nested AP
    rptrNested = wptrNested - m_delayNested;
    if (rptrNested < 0)
    {
        rptrNested+=m_maxdelay; // if it’s less than 0, wrap it round
    }
    
    rptr = wptr - m_delay; // AP1
    if (rptr < 0)
    {
        rptr+=m_maxdelay; // if it’s less than 0, wrap it round
    }
    
    
    //AP1
    d_out = dline[rptr];
    Vn = in - d_out*m_alpha; // calculate sample after the first summation (v(n))
    out = Vn*m_alpha + d_out; // calculate the output AP1
    
    
    //Nested AP2
    d_outNested = dlineNested[rptrNested]; // output from delay line Nested 2
    Tn = Vn - d_outNested*m_alphaNested; // calculate sample after the first summation (T(n))
        
    outNested = Tn*m_alpha + d_outNested; // calculate the output

    //UPDATE Storage for Nested 2
    dlineNested[wptrNested] = Tn; // update the delay line for AP Nested 2 with T(n)
    
    
    //UPDATE Storage for Nested 1
    dline[wptr] = outNested; // update the delay line with output of Nested 2
    
    
    wptr++; // add one to the write pointer
    if(wptr>=m_maxdelay)
    {
        wptr = 0; // wrap it back to zero if it’s gone bigger than max delay
    }
    
    
    //increment write pointer
    wptrNested++; // add one to the write pointer
    if(wptrNested>=m_maxdelay)
    {
        wptrNested = 0; // wrap it back to zero if it’s gone bigger than max delay
    }
    
    return out;
}


void NestedAllPassReverb::cleanNestedBuffer(){
    if(dlineNested)
    {
        delete [] dlineNested;
        dlineNested = nullptr;
     }
}


void NestedAllPassReverb::allocateNestedBuffer(){
    dlineNested = new double[m_maxdelay]; // dynamically allocate 48000 samples
    memset(dlineNested,0,m_maxdelay*sizeof(double));
}
