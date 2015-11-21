/* 
 * Neurons - connectivityMatrix
 *
 * This is a four neuron network. Two sensory, two motor.
 *
 * It implements a kind of filter over the previous twoNeuronNetwork. Whereas in that
 * example, the two sensory neurons acted directly like motor neurons, here, the sensory
 * neurons aren't used directly. The connectivity matrix implements a kind of linear 
 * combination of the sensory neuron spikes, which drive motor neurons. 
 *
 * The motor neurons' outputs aren't converted to motor commands in this example, for the sake 
 * of clarity. But, the 'Visualize' function (in 'void loop()') allows you to get an idea of 
 * the spiking behaviour by interfacing LEDs to the default output pins.
 *
 */


#include <Neurons.h>
#include <Network.h>


//Network variables
int sensoryNeuronIDs[2] = {0,1};
int connectivity[16] = {0  , 0  , 100,-800, // Note that the matrix is actually one-dimensional, it's expanded here for clarity. 
                        0  , 0  ,-800, 100, // The rows correspond to the spiking neuron and the columns the neurons on the reciving end.
                        0  , 0  , 0  , 0  , // You could, in principle, implement a fully connected network with this matrix.
                        0  , 0  , 0  , 0}; 
                                          
                                         
Network testNetwork(4,connectivity); // The constructor takes the connectivity matrix as an input too, if you have one


//Interrupt variables
static volatile bool pins[2] = {0,1};


void setup(){
  Serial.begin(9600);
  testNetwork.dt = 5;
  testNetwork.setSensoryNeurons(sensoryNeuronIDs, 2);
}

void loop() {
  testNetwork.updateNeurons();
  testNetwork.issueSpikes();
  
  testNetwork.Visualize();
  delay(testNetwork.dt);
}


ISR(PCINT1_vect){
  if(digitalRead(A0)!=pins[0]){
    testNetwork.Neurons[0].inputCurrent += 2000;
    pins[0] = digitalRead(A0);
  }
  if(digitalRead(A1)!=pins[1]){
    testNetwork.Neurons[1].inputCurrent += 2000;
    pins[1] = digitalRead(A1);
  }
}


