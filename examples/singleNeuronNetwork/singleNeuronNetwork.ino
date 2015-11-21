/* 
 * Neurons - singleNeuronNetwork
 * 
 * This example illustrates the use of a network to drive motor outputs. This particular
 * network contains only one neuron, and serves as an example to make the transition from single
 * neurons to connected networks easier.
 * 
 * The one neuron is both sensory and motor. It takes input from the default input pin for 
 * neuron 0 (A0, set for Pin Change Interrupt); it sends output spikes from the default output 
 * pin for neuron 0 (pin 7). For a complete list of these default pins, see lines 11(default 
 * output) and 12 (default input) of Network.cpp.
 * 
 */

#include <Neurons.h>
#include <Network.h>


//Network variables
Network testNetwork(1); // Network constructor.
int sensoryNeuronIDs[1] = {0}; // The array should contain the indexes of the sensory neurons. Useful later, when we 'setSensoryNeurons'.

         
//Interrupt variables
/* Sensory input is best taken in asynchronously (all other neurons' membrane
 *  potentials and spikes are updated synchronously). 
 * Since the sensory neurons expect inputs at the PinChange Interrupt Pins, we 
 *  need the following boolean array to check the state of the sensory pins.
 */
static volatile bool pins[1] = {0};


//Output Control Variables
/* It helps to specify the indices of the neurons whose spikes are supposed to cause motor
 *  output. For example, the zeroeth neuron (the only neuron in this network), is 
 *  supposed to cause the robot to move forward: 
 * The boolean 'commandSent' variable is required to ensure that the command is 
 *  sent only once per spike.
 */
int forwardNeuron = 0;
volatile boolean commandSent=1;


void setup(){
  Serial.begin(9600);
  
  //Network stuff
  testNetwork.dt = 5; //Update rate of the network (in milliseconds)
  testNetwork.setSensoryNeurons(sensoryNeuronIDs, 1); // Using the array from above to know which pins are sensory
  
 /* Motor output will be sent from thes pins. In this case, the pins were
  *  connected to an L293D motor driver IC, and the pins and values are set
  *  accordingly. You may need to change this according to your setup.
  */ 
  //Left wheel
  pinMode(8,OUTPUT); digitalWrite(8,LOW);
  pinMode(9,OUTPUT); digitalWrite(9,LOW);
  //Right Wheel
  pinMode(10,OUTPUT); digitalWrite(10,HIGH);
  pinMode(11,OUTPUT); digitalWrite(11,HIGH);
}

void loop() {
  /* 
   * Every iteration of the loop indicates one time-step. Within each time-step,
   * a number of tasks occur, each with it's own dedicated code block in the code
   * below:
   * 
   * 1. The neurons' membrane potentials are updated, and if spikes occur
   *    then they are relayed throughout the network.
   * 2. The requisite instructions are issued, according to the spiking of the motor
   *    neurons.
   * 3. The outputs of the neurons are sent though the output pins, from where they can
   *    be visualized on LEDs. This is an optional step.
   *    
   * The neuron's input pins are continuously monitored in the background, triggering the interrupt
   * routine whenever a change occurs on any one of them.
   * 
   */


  /* This code block is where the synchronous update of the neurons' membrane potentials 
   *  and spikes takes place. This part of the code remains pretty much constant.
   */
  testNetwork.updateNeurons();
  testNetwork.issueSpikes();
 
  /* This block ensures that a command sent as a response to a motor neuron spike issued in the previous 
   *  time-step doesn't persist into the next time-step.
   * The last if-statement is is where the commands are actually sent, according to the spiking 
   *  of the motor neurons.
   */
  if(commandSent){
    digitalWrite(8,LOW);
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    digitalWrite(11,LOW);
    commandSent=0;
  }
  if(testNetwork.Neurons[forwardNeuron].spike) moveForward();
 
  /* The 'Visualize' function is used to send the required outputs to the default output pins 
   *  of the neurons (these default pins are assigned according to the neuron index, see line 
   *  11 of Network.cpp)
   */
  testNetwork.Visualize();

  /* This delay is actually very important. It allows for the sensory neurons to 'accumulate'
   *  input current from their input pins before the next calculation of the nest time-step commence.
   */
  delay(testNetwork.dt);
}


/* This interrupt routine relays asynchronous spikes coming in at the analogPins to the sensoryNeurons.
 * Notice the input pin is A0 - the default for neuron index 0. This is another part of the code that
 *  remains pretty much constant. The same block repeats with more neurons.
 */
ISR(PCINT1_vect){
  if(digitalRead(A0)!=pins[0]){
    testNetwork.Neurons[0].inputCurrent += 2000;
    pins[0] = digitalRead(A0);
  }
}

/* This function issues the instructions required by the associated motor neuron.
 *  In this case, that would be to make the robot turn right.
 */
void moveForward(){
  digitalWrite(8,LOW);
  digitalWrite(9,HIGH);
  digitalWrite(10,LOW);
  digitalWrite(11,HIGH);
  commandSent=1;  
}



