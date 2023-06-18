/*
  ==============================================================================

    ReverbModule.h
    Created: 17 Jun 2023 4:17:19pm
    Author:  Oscar Ortega

  ==============================================================================
*/

#pragma once
#include <iostream>



class ReverbModule
{

public:
    ReverbModule(){
        //std::cout<< "Constructor Reverb Module" << std::endl;

        rptr = wptr = 0;
        m_maxdelay = 48000; // 1 sec max
        m_delay = 8000; // how many samples delay?
        cleanBuffer();
        allocateBuffer();
    };
    
    virtual void changeDelayLength(int sample){
        rptr = wptr = 0;
        m_maxdelay = (int)sample; // 1 sec max
        cleanBuffer();
        allocateBuffer();
        
    };
    
    void changeMDelay(double sample) {
        
        double temp = (sample*0.001)*m_maxdelay;
        m_delay=(int)temp;
    };
    
    virtual double process(double inL) = 0;
    virtual ~ReverbModule(){
        //std::cout<< "Destructor Reverb Module" << std::endl;
        cleanBuffer();
    };
    
protected:
    int m_delay; //Delay time
    int m_maxdelay; //max delay time
    int rptr,wptr; // Pointers
    double *dline = nullptr; //Buffer
    
    
private:
    void cleanBuffer(){
        if(dline)
        {
            delete [] dline;
            dline = nullptr;
         }
    }
    
    void allocateBuffer(){
        dline = new double[m_maxdelay]; // dynamically allocate XXXX samples
        memset(dline,0,m_maxdelay*sizeof(double));
    }
};
