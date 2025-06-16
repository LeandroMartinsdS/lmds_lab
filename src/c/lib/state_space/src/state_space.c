#include <stdio.h>
#include "state_space.h"

void init_state_space(StateSpaceSystem *sys, float *x_init,
                      float *A, float *B, float *C, float *D,
                      size_t states, size_t inputs , size_t outputs) {

    if (sys == NULL || A == NULL || B == NULL || C == NULL || D == NULL || x_init == NULL) {
        fprintf(stderr, "Error: Null pointer passed to init_state_space.\n");
        return;
    }
    if (states <= 0 || inputs <= 0 || outputs <= 0) {
        fprintf(stderr, "Error: Invalid dimensions for state space system.\n");
        return;
    }

    sys->A = A;
    sys->B = B;
    sys->C = C;
    sys->D = D;
    sys->x = x_init;
    sys->states = states;
    sys->inputs = inputs;
    sys->outputs = outputs;

    // Ensure the matrices are properly sized
    if (sizeof(A) / sizeof(A[0]) != states * states || sizeof(B) / sizeof(B[0]) != states * inputs ||
        sizeof(C) / sizeof(C[0]) != outputs * states || sizeof(D) / sizeof(D[0]) != outputs * inputs ||
        sizeof(x_init) / sizeof(x_init[0]) != states) {
        fprintf(stderr, "Error: Matrix dimensions do not match the specified sizes.\n");
        return;
    }
}

void print_state_space(const StateSpaceSystem *sys) {
    printf("State Space System:\n");
    printf("States: %lu\n", sys->states);
    printf("Inputs: %lu\n", sys->inputs);
    printf("Outputs: %lu\n", sys->outputs);

    printf("A matrix:\n");
    for (int i = 0; i < sys->states; i++) {
        for (int j = 0; j < sys->states; j++) {
            printf("%f ", sys->A[i * sys->states + j]);
        }
        printf("\n");
    }

    printf("B matrix:\n");
    for (int i = 0; i < sys->states; i++) {
        for (int j = 0; j < sys->inputs; j++) {
            printf("%f ", sys->B[i * sys->inputs + j]);
        }
        printf("\n");
    }

    printf("C matrix:\n");
    for (int i = 0; i < sys->outputs; i++) {
        for (int j = 0; j < sys->states; j++) {
            printf("%f ", sys->C[i * sys->states + j]);
        }
        printf("\n");
    }

    printf("D matrix:\n");
    for (int i = 0; i < sys->outputs; i++) {
        for (int j = 0; j < sys->inputs; j++) {
            printf("%f ", sys->D[i * sys->inputs + j]);
        }
        printf("\n");
    }

    printf("Initial state x:\n");
    for (int i = 0; i < sys->states; i++) {
        printf("%f ", sys->x[i]);
    }
    printf("\n");
}