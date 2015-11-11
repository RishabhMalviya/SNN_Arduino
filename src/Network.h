#ifndef Network_h
#define Network_h

#include "Arduino.h"
#include "Neurons.h"

#define MAX_NEURONS 6

class Network{
  private:
    bool sensoryNeurons[MAX_NEURONS]; //only neurons with inputPins.
    void setConnectivityMatrix(int connectivity[]);

  public:
    Neuron Neurons[MAX_NEURONS];
    int neuronCount;
    int sensoryNeuronCount;
    int connectivityMatrix[MAX_NEURONS][MAX_NEURONS];
    static const int defaultOutPins[MAX_NEURONS];// = {7, 4, 2, 1, 0, 6};
    static const int defaultInputPins[MAX_NEURONS];// = {A0, A1, A2, A3, A4, A5};
    static int dt;

    Network(int numberOfNeurons);
    Network(int numberOfNeurons, int connectivity[]);
    ~Network();

    void setSensoryNeurons(int* sensoryNeuronIDs, int sensoryNeuronCount_);
    void issueSpikes();
    void updateNeurons(); //calls 'calculateMembranePotential' for each Neuron
    void Visualize();
};     

#endif Network_h
