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
    /*
     * Only sensory neurons have an inputPin. 
     * Every neuron toggles it's outputPin when it spikes. 
     * The outputPin is connectected to the spikePin.
     * spikePins trigger pinChangeInterrupts that cause the downstream neurons to increment their currents.
    */
    int inputPin, outputPin; 
    float inputCurrent, membranePotential;
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
