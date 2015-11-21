/*
  Network.cpp - Library for network of LIF neurons in Arduino.
  Created by Rishabh Malviya, October 17, 2015.
  Released into the public domain.
*/

#include "Network.h"

#define MAX_NEURONS 6

const int Network::defaultOutPins[MAX_NEURONS] = {7, 6, 5, 4, 3, 2};
const int Network::defaultInputPins[MAX_NEURONS] = {A0, A1, A2, A3, A4, A5};
int Network::dt;
/*CONSTRUCTORS*/

//Default Constructor
Network::Network(int numberOfNeurons){
  //Sanity check
    if(numberOfNeurons>MAX_NEURONS) exit;

  //Set neuronCount
    neuronCount = numberOfNeurons;

  //Initialize Neurons with default output pins
    for(int i=0; i<numberOfNeurons; i++){
      Neurons[i].setOutputPin(defaultOutPins[i]);
      pinMode(defaultOutPins[i],OUTPUT);
    }

  //Re-configure everything to pinChangeInterrupts
    cli(); //disable interrupts during interrupt configuration

    EIFR |= 0b00000011; //Clearing externalInterrupt flags.
    EIMSK &= 0b11111100; //Disabling externalInterrupts

    PCIFR |= 0b00000111; //Clearing pinChangeInterrupt flags
    PCICR &= 0b11111000; //Disabling all pinChangeInterrupts

    sei();
  
};


//Constructor with connectivity matrix
Network::Network(int numberOfNeurons, int connectivity[]){
  //Sanity check
    if(numberOfNeurons > MAX_NEURONS) exit;
    if((sizeof(connectivity)/sizeof(connectivity[0])) != (numberOfNeurons*numberOfNeurons)) exit;

  //Initialize Variables
    neuronCount = numberOfNeurons;

  //Initialize Neurons with default output pins
    for(int i=0; i<numberOfNeurons; i++){
      Neurons[i].setOutputPin(defaultOutPins[i]);
      pinMode(defaultOutPins[i],OUTPUT);
    }

  //Re-configure everything to pinChangeInterrupts
    cli(); //disable interrupts during interrupt configuration

    EIFR |= 0b00000011; //Clearing externalInterrupt flags.
    EIMSK &= 0b11111100; //Disabling externalInterrupts

    PCIFR |= 0b00000111; //Clearing pinChangeInterrupt flags
    PCICR &= 0b11111000; //Disabling all pinChangeInterrupts

    sei();

  //Set Connectivity Matrix
    setConnectivityMatrix(connectivity);

};



/*PRIVATE MEMBER FUNCTIONS*/

void Network::setConnectivityMatrix(int connectivity[]){
  for(int i=0; i<neuronCount; i++){
    for(int j=0; j<neuronCount; j++){
      connectivityMatrix[i][j] = connectivity[i*neuronCount + j];
    }
  }
};



/* PUBLIC MEMBER FUNCTIONS */

void Network::setSensoryNeurons(int* sensoryNeuronIDs, int sensoryNeuronCount_){
  //Sanity check
    if(sensoryNeuronCount_>neuronCount) exit;
    
  //Set sensoryNeuronCount
    sensoryNeuronCount = sensoryNeuronCount_;

  //Enable interrupts for sensory neurons
    bitClear(PCICR,1);
    for(int i=0; i<sensoryNeuronCount; i++){
      bitSet(PCMSK1,sensoryNeuronIDs[i]);
      pinMode(defaultInputPins[sensoryNeuronIDs[i]],INPUT);
      digitalWrite(defaultInputPins[sensoryNeuronIDs[i]],HIGH); //Stabilising through internal pull-up resistor
    }
    bitSet(PCICR,1);
};

void Network::updateNeurons(){
  for(int i=0; i<neuronCount; i++){
    Neurons[i].calculateMembranePotential(dt);
  }

};

void Network::issueSpikes(){
  for(int neuronID=0; neuronID<neuronCount; neuronID++){
    if(Neurons[neuronID].spike){ 
      for(int i=0; i<neuronCount; i++){
        Neurons[i].inputCurrent += (connectivityMatrix[neuronID][i])*200;
      }
    }
  }
};

void Network::Visualize(){
  for(int neuronID=0; neuronID<neuronCount; neuronID++){
    Neurons[neuronID].LED_Output(defaultOutPins[neuronID]);
  }
};


/* DESTRUCTORS */

//Default Destructor
Network::~Network(){

}