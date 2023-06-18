//
//  DelayLine.hpp
//  Reverb_VST
//
//  Created by Oscar Ortega on 16/12/20.
//

#ifndef DelayLine_h
#define DelayLine_h

#include <stdio.h>
#include "ReverbModule.h"

class DelayLine : public ReverbModule
{

public:
    DelayLine();
    double process(double inL) override;
    virtual ~DelayLine();

};
#endif /* DelayLine_hpp */
