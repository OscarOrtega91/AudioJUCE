//
//  DeZipper.hpp
//  Reverb_VST
//
//  Created by Oscar Ortega on 06/01/21.
//

#ifndef DeZipper_h
#define DeZipper_h

#include <stdio.h>
class DeZipper{
public:
    DeZipper();
    double smooth(double sample);
    
    
private:
    float m_DZMM;  //single sample delay memory
    float m_DZFB;  //Feedback coef
    float m_DZFF;  //Feed Forward coef
};
#endif /* DeZipper_hpp */
