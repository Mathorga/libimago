#include "standard.h"

void ccol_init(corticolumn* column, uint32_t neurons_count) {
    dccol_init(column, neurons_count, 10);
}

void dccol_init(corticolumn* column, uint32_t neurons_count, uint16_t synapses_density) {
    // Randomize seed.
    // Comment this if you need consistent result across multiple runs.
    // srand(time(NULL));

    // Allocate neurons.
    column->neurons_count = neurons_count;
    column->neurons = (neuron*) malloc(column->neurons_count * sizeof(neuron));

    // Allocate synapses.
    column->synapses_count = synapses_density * neurons_count;
    column->synapses = (synapse*) malloc(column->synapses_count * sizeof(synapse));

    // Initialize neurons with default values.
    for (uint32_t i = 0; i < column->neurons_count; i++) {
        column->neurons[i].threshold = DEFAULT_THRESHOLD;
        column->neurons[i].value = STARTING_VALUE;
    }

    // Initialize synapses with random values.
    for (uint32_t i = 0; i < column->synapses_count; i++) {
        // Assign a random input neuron.
        int32_t randomInput = rand() % column->neurons_count;

        // Assign a random output neuron, different from the input.
        int32_t randomOutput;
        do {
            randomOutput = rand() % column->neurons_count;
        } while (randomOutput == randomInput);

        column->synapses[i].input_neuron = randomInput;
        column->synapses[i].output_neuron = randomOutput;
        column->synapses[i].propagation_time = DEFAULT_PROPAGATION_TIME;
        column->synapses[i].value = DEFAULT_VALUE;
    }

    // Initialize spikes data.
    column->spikes_count = 0;
}

void ccol_feed(corticolumn* column, uint32_t* target_neurons, uint32_t targets_count, int8_t value) {
    if (targets_count > column->neurons_count) {
        // TODO Handle error.
        return;
    }

    for (uint32_t i = 0; i < targets_count; i++) {
        column->neurons[target_neurons[i]].value += value;
    }
}

void ccol_propagate(corticolumn* column) {
    // Loop through spikes.
    for (uint32_t i = 0; i < column->spikes_count; i++) {
        // Retrieve current spike.
        spike* current_spike = &(column->spikes[i]);

        // Retrieve reference synapse.
        synapse* reference_synapse = &(column->synapses[current_spike->synapse]);

        if (current_spike->progress < reference_synapse->propagation_time &&
            current_spike->progress != SPIKE_DELIVERED) {
            // Increment progress if less than propagationTcolumnime and not alredy delivered.
            current_spike->progress++;
        } else if (current_spike->progress >= reference_synapse->propagation_time) {
            // Set progress to SPIKE_DELIVERED if propagation time is reached.
            current_spike->progress = SPIKE_DELIVERED;
        }
    }
}

void ccol_increment(corticolumn* column) {
    // Loop through neurons.
    for (uint32_t i = 0; i < column->neurons_count; i++) {
        // Retrieve current neuron.
        neuron* current_neuron = &(column->neurons[i]);

        // Make sure the neuron value does not go below 0.
        if (current_neuron->value > 0) {
            // Decrement value by decay rate.
            current_neuron->value -= DECAY_RATE;
        }

        // Loop through spikes.
        for (uint32_t j = 0; j < column->spikes_count; j++) {
            // Retrieve current spike.
            spike* current_spike = &(column->spikes[j]);

            // Retrieve reference synapse.
            synapse* reference_synapse = &(column->synapses[current_spike->synapse]);

            // Only increment neuron value if spike is delivered and synapse outputs to the current neuron.
            if (reference_synapse->output_neuron == i && current_spike->progress == SPIKE_DELIVERED) {
                current_neuron->value += reference_synapse->value;
            }
        }
    }
}

void ccol_fire(corticolumn* column) {
    // Loop through neurons and fire those whose value exceeded their threshold.
    for (uint32_t i = 0; i < column->neurons_count; i++) {
        if (column->neurons[i].value > column->neurons[i].threshold) {
            // Loop through synapses and fire them.
        }
    }
}

void ccol_tick(corticolumn* column) {
    // Update synapses.
    ccol_propagate(column);

    // Update neurons.
    ccol_increment(column);

    // Fire neurons.
    ccol_fire(column);
}