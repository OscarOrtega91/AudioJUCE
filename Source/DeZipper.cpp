//
//  DeZipper.cpp
//  Reverb_VST
//
//  Created by Oscar Ortega on 06/01/21.
//

#include "DeZipper.h"

#define TOOSMALL 0.0000000000000000000000001f

DeZipper::DeZipper()

{
    m_DZMM = 0.0;
    m_DZFB = 0.999;//FB Coef
    m_DZFF = 1-m_DZFB;//FF Coef for 0dB passband gain
}

//-----------------------------------------------------

double DeZipper::smooth(double sample)
{
    double temp = m_DZFF*sample + m_DZFB*m_DZMM + TOOSMALL;
        
    m_DZMM = temp;
    return temp;
}
