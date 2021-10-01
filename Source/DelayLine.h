//
//  DelayLine.hpp
//  Reverb_VST
//
//  Created by Oscar Ortega on 16/12/20.
//

#ifndef DelayLine_h
#define DelayLine_h

#include <stdio.h>


class DelayLine
{

public:
    DelayLine();
    void changeDelayLength(int sample);
    void changeMDelay(double sample);
    double process(double inL);
    ~DelayLine();
private:
    int m_delay; //Delay time
    int m_maxdelay; //max delay time
    int rptr,wptr;
    double *dline = nullptr;
};
#endif /* DelayLine_hpp */
