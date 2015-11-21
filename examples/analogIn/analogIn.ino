/*
 * Neurons - analogIn
 * 
 * This example shows how to drive a single neuron independent of a network, here
 * only the Neuron class is used) through an incoming analog input voltage. 
 * 
 * All other example involving networks use neurons driven by 'spiking' sensors.
 * These sensors cause digital pin changes at the neurons' assigned input pins and 
 * are detected using pin change interrupts.
 * 
 */

#include <Neurons.h>

int dt = 100; //in milliseconds
int neuronOutputPin = 9; //you can visualize the neuron's response by connecting an LED to this pin
int neuronInputPin = A1; //this is where the input will come in from
Neuron testNeuron(neuronInputPin, neuronOutputPin);
  
void setup(){
  pinMode(testNeuron.outputPin,OUTPUT);
  pinMode(testNeuron.inputPin, INPUT);
  Serial.begin(9600);
}

void loop(){
  testNeuron.inputCurrent = analogRead(testNeuron.inputPin)/3; //You can change the scaling here from 1/3 to something else for a different neuron sensitivity
  
  testNeuron.calculateMembranePotential((float)dt);
  testNeuron.LED_Output(testNeuron.outputPin);
  delay(dt);
}


