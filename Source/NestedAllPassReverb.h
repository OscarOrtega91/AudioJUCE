//
//  NestedAllPassReverb.hpp
//  Reverb_VST
//
//  Created by Oscar Ortega on 16/12/20.
//

#ifndef NestedAllPassReverb_h
#define NestedAllPassReverb_h

#include <stdio.h>
#include "ReverbModule.h"

class NestedAllPassReverb : public ReverbModule
{

public:
    NestedAllPassReverb();
    void changeDelayLength(int sample) override;
    void changeMDelayNested(double delay);
    double process(double inL) override;
    virtual ~NestedAllPassReverb();

private:
    int m_delayNested; //Delay time Nested
    int rptrNested,wptrNested; //Pointers for Nested All Pass

    double m_alpha; //feedback gain (f [slider])
    double m_alphaNested; //feedback gain NEsted (f [slider])
    double *dlineNested=nullptr;

    void cleanNestedBuffer();
    void allocateNestedBuffer();
};

#endif /* NestedAllPassReverb_hpp */
