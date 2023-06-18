//
//  LowPassFB.hpp
//  Reverb_VST
//
//  Created by Oscar Ortega on 21/12/20.
//

#ifndef LowPassFB_h
#define LowPassFB_h

#include <stdio.h>
#include "ReverbModule.h"

class LowPassFB : public ReverbModule
{
public:
    LowPassFB();
    virtual ~LowPassFB();
    double process(double in) override;
    
    private :

    double m_fbGain;
    double M_OPFB;
    double M_OPFF;
    double M_OPMM;
    
    
};
#endif /* LowPassFB_hpp */
