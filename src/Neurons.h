#ifndef Neuron_h
#define Neuron_h

#include "Arduino.h"

class Neuron{
  private:
    float C;
    float gL;
    float EL;
    float VT;
    float Vspike;
    
   public:
    int inputPin, outputPin; 
    float membranePotential;
    long int inputCurrent;
    boolean spike;    
    
    Neuron();
    Neuron(int outputPin_);
    Neuron(int inputPin_, int outputPin_);
    ~Neuron();
    void setInputPin(int inputPin_);
    void setOutputPin(int outputPin_);
    void calculateMembranePotential(float dt);
    void LED_Output(int outPin);
    void setLeakConstant(float gL_);
};     

#endif Neuron_h
