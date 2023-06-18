//
//  AllPassReverbDamping.hpp
//  Reverb_VST
//
//  Created by Oscar Ortega on 23/12/20.
//

#ifndef AllPassReverbDamping_h
#define AllPassReverbDamping_h

#include <stdio.h>
#include <iostream>
#include "ReverbModule.h"

class AllPassReverbDamping : public ReverbModule
{

public:
    AllPassReverbDamping();
    //void changeDelayLength(int sample);
    //void changeMDelay(double sample);
    
    double process(double inL) override;
    virtual ~AllPassReverbDamping();
private:
    //int m_delay; //Delay time
    //int m_maxdelay; //max delay time
    //int rptr,wptr;
    //double *dline=nullptr;

    double m_alpha; //feedback gain (f [slider])
    double m_fbGain;
    double M_OPFB;
    double M_OPFF;
    double M_OPMM;
};

#endif /* AllPassReverbDamping_hpp */
