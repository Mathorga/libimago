/*
*****************************************************************
imago.h

Copyright (C) 2021 Luka Micheletti
*****************************************************************
*/

#ifndef __IMAGO__
#define __IMAGO__

#include <stdint.h>
#include <stdlib.h>

#define SPIKE_DELIVERED -1
#define SPIKE_IDLE -2

#define DECAY_RATE 1

// Default threshold value is 0.8.
#define DEFAULT_THRESHOLD 0xCCu

struct Neuron {
    // Threshold value. The neuron fires if value goes above it.
    uint8_t threshold;

    // Actual value of the neuron. If it goes above threshold, then the neuron fires.
    uint8_t value;
};

struct Synapse {
    // Propagation time of spikes along the synapse.
    uint8_t propagationTime;

    // Progress of the current spike along the synapse.
    int16_t progress;

    // Value of the synapse. This is what influences the output neuron.
    int8_t value;

    // Index of the input neuron.
    uint32_t inputNeuron;

    // Index of the output neuron.
    uint32_t outputNeuron;
};

// Defines the building block of the brain intelligence: the minimum sensory-motor learning model.
struct Corticolumn {
    // The number of neuron in the corticolumn (also defines the number of synapses).
    uint32_t neuronsNum;

    // Actual neurons in the corticolumn. The size is defined by neuronsNum.
    struct Neuron* neurons;

    // Amount of synapses in the corticolumn.
    uint32_t synapsesNum;

    // Synapses in the corticolumn. This size is defined by synapsesNum.
    struct Synapse* synapses;
};

// Initializes the given Corticolumn with default values.
void initColumn(struct Corticolumn* column, uint32_t neuronsNum);

// Propagates synapse spikes to post-synaptic neurons.
void propagate(struct Corticolumn* column);

// Increments neuron values with spikes from input synapses and decrements them by decay.
void increment(int8_t* neuronValues,
               uint32_t** neuronInputs,
               uint32_t* synapseIndexes,
               int8_t* synapseValues,
               uint8_t* spikeProgresses,
               uint32_t neuronsNum,
               uint32_t synapsesNum);

// Triggers neuron firing if values exceeds threshold.
void fire();

// Copies a whole corticolumn to device in order to run kernels on it.
void copyToDevice();

// Performs a full cycle over the network corticolumn.
void tick(struct Corticolumn* column);

#endif