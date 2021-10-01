//
//  LowPassFB.hpp
//  Reverb_VST
//
//  Created by Oscar Ortega on 21/12/20.
//

#ifndef LowPassFB_h
#define LowPassFB_h

#include <stdio.h>

class LowPassFB
{
public:
    LowPassFB();
    ~LowPassFB();
    void changeDelayLength(int sample);
    void changeMDelay(double delay);
    double process(double in);
    
    private :
    int m_delay;
    int m_maxdelay;
    int rptr,wptr;
    double *dline=nullptr;
    double m_fbGain;
    double M_OPFB;
    double M_OPFF;
    double M_OPMM;
    
    
};
#endif /* LowPassFB_hpp */
