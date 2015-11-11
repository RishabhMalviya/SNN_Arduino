/*
 * An example where a single neuron is driven by analog input (acting like the 
 * pre-synaptic current coming into the neuron's axon.
 */


#include <Neurons.h>

int neuronOutputPin = 9;
int neuronInputPin = A1;
Neuron testNeuron(neuronInputPin, neuronOutputPin);
  
void setup(){
  pinMode(testNeuron.outputPin,OUTPUT);
  pinMode(testNeuron.inputPin, INPUT);
  Serial.begin(9600);
}

void loop(){
  
  testNeuron.inputCurrent = analogRead(testNeuron.inputPin)/3;
  
  testNeuron.calculateMembranePotential((float)testNeuron.dt);
  testNeuron.LED_Output(testNeuron.outputPin);
  delay(testNeuron.dt);
}
