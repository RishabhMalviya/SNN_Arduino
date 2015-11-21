/* 
 * Neurons - twoNeuronNetwork
 * 
 * This example uses two sensory-motor Neurons.
 *
 * One sound sensor is mounted on the left side of the robot, the other on the right. 
 * The two neurons are interfaced with the sound sensors, and also act as motor neurons. 
 * When the right sensor spikes, the robot turns right. When the left one spikes the 
 * robot turns left. 
 * 
 * Though significantly different in terms of functionality, the code is almost the same
 * as in the single neuron case. Code blocks that are different from the singleNeuronNetwork 
 * example are marked with a '//!!//'.
 * 
 */

#include <Neurons.h>
#include <Network.h>

//!!//
//Network variables
int sensoryNeuronIDs[2] = {0,1};
Network testNetwork(2);

//!!//
//Interrupt variables
static volatile bool pins[2] = {0,1};

//!!//
//Output Control Variables
int leftTurnNeuron = 0, rightTurnNeuron = 1;
volatile boolean commandSent=1;

void setup(){
  Serial.begin(9600);

  //!!//
  //Network stuff
  testNetwork.dt = 5;
  testNetwork.setSensoryNeurons(sensoryNeuronIDs, 2);
  
  //Left wheel
  pinMode(8,OUTPUT); digitalWrite(8,LOW);
  pinMode(9,OUTPUT); digitalWrite(9,LOW);
  //Right Wheel
  pinMode(10,OUTPUT); digitalWrite(10,HIGH);
  pinMode(11,OUTPUT); digitalWrite(11,HIGH);
}

void loop() {
  testNetwork.updateNeurons();
  testNetwork.issueSpikes();

  //!!//
  if(commandSent){
    digitalWrite(8,LOW);
    digitalWrite(9,LOW);
    digitalWrite(10,LOW);
    digitalWrite(11,LOW);
    commandSent=0;
  }
  if(testNetwork.Neurons[rightTurnNeuron].spike) turnRight();
  if(testNetwork.Neurons[leftTurnNeuron].spike) turnLeft();
 
  testNetwork.Visualize();
  delay(testNetwork.dt);
}


//!!//
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

void turnRight(){
  digitalWrite(8,LOW);
  digitalWrite(9,HIGH);
  digitalWrite(10,LOW);
  digitalWrite(11,HIGH);
  commandSent=1;  
}

//!!//  
void turnLeft(){
  digitalWrite(8,HIGH);
  digitalWrite(9,LOW);
  digitalWrite(10,HIGH);
  digitalWrite(11,LOW);
  commandSent=1; 
}



