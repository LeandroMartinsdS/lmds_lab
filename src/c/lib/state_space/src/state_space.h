#ifndef STATE_SPACE_H
#define STATE_SPACE_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h> // for size_t

typedef struct {
    float *A;       // State matrix
    float *B;       // Input matrix
    float *C;       // Output matrix
    float *D;       // Direct transmission matrix
    float *x;       // State vector
    // float *u;    // Input vector
    // float *y;    // Output vector

    size_t states;     // number of states
    size_t inputs;     // number of inputs
    size_t outputs;    // number of outputs

} StateSpaceSystem;

void init_state_space(StateSpaceSystem *sys, float *x_init,
                      float *A, float *B, float *C, float *D,
                      size_t states, size_t inputs, size_t outputs);

void print_state_space(const StateSpaceSystem *sys);

#endif // STATE_SPACE_H