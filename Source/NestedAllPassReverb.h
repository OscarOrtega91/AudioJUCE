//
//  NestedAllPassReverb.hpp
//  Reverb_VST
//
//  Created by Oscar Ortega on 16/12/20.
//

#ifndef NestedAllPassReverb_h
#define NestedAllPassReverb_h

#include <stdio.h>

class NestedAllPassReverb
{

public:
    NestedAllPassReverb();
    void changeDelayLength(int sample);
    void changeMDelay(double delay);
    void changeMDelayNested(double delay);
    double process(double inL);
    ~NestedAllPassReverb();

private:
    int m_delay; //Delay time
    int m_delayNested; //Delay time Nested
    int m_maxdelay; //max delay time
    int rptr,wptr; //Pointers for All Pass 1
    int rptrNested,wptrNested; //Pointers for Nested All Pass

    double m_alpha; //feedback gain (f [slider])
    double m_alphaNested; //feedback gain NEsted (f [slider])
    double *dline=nullptr;
    double *dlineNested=nullptr;

};

#endif /* NestedAllPassReverb_hpp */
