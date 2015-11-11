/*
  Neurons.cpp - Library for implementng LIF neurons in Arduino code.
  Created by Rishabh Malviya, October 17, 2015.
  Released into the public domain.
*/

#include "Neurons.h"


//Default constructor
Neuron::Neuron(){
  membranePotential = -60.0;
  C = 2000.0;
  gL = 5.0/12.0;
  EL = -60.0;
  VT = 20.0;
  Vspike = 60.0;
  spike = false;
    
  inputCurrent = 0;
}

//Output Neuron constructor
Neuron::Neuron(int outputPin_){
    membranePotential = -60.0;
    C = 2000.0;
    gL = 5.0/12.0;
    EL = -60.0;
    VT = 20.0;
    Vspike = 60.0;
    spike = false;
    
    inputCurrent = 0;
    outputPin = outputPin_;
}
  
//Input and Output Neuron Constructor
Neuron::Neuron(int inputPin_, int outputPin_){
  membranePotential = -60.0;
  C = 2000.0;
  gL = 5.0/12.0;
  EL = -60.0;
  VT = 20.0;
  Vspike = 60.0;
  spike = false;
    
  inputCurrent = 0;
  inputPin = inputPin_;
  outputPin = outputPin_;
}
 


//Set Input Pin
void Neuron::setInputPin(int inputPin_){
    inputPin = inputPin_;
}
  
//Set Output Pin
void Neuron::setOutputPin(int outputPin_){
  outputPin = outputPin_;
}  


//Membrane Potential Update Function
void Neuron::calculateMembranePotential(float dt){
  float deltaMembranePotential = 0;
  
  if(spike){
    membranePotential = VT-40.0;
    spike = false;
  }
  
  deltaMembranePotential = (inputCurrent - gL*(membranePotential-EL))*(dt/C);
  membranePotential += deltaMembranePotential;

  if(membranePotential>VT){
    membranePotential = Vspike;
    spike = true;
  }
  
  if(membranePotential<-60.0){
    membranePotential = -60.0;
  }
  
  inputCurrent = 0;
}

//Visualizing Neuron Output Through LED
void Neuron::LED_Output(int outPin){  
  boolean output = 0;
  
  if(!spike){
    //output = ((membranePotential-EL)*(110/120));
    output = 0;
  }
  else if(spike){
    output = 255;
  }

  digitalWrite(outPin,(int)output);
}

void Neuron::setLeakConstant(float gL_){
  gL = gL_;
};


//Destructor
Neuron::~Neuron(){
  
}