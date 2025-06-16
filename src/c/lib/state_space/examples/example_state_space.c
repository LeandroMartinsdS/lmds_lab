#include "example_state_space.h"

void example_state_space(void) {
    StateSpaceSystem sys;
    float A[] = {1.0, 0.0, 0.0, 1.0}; // Example state matrix
    float B[] = {0.0, 1.0};           // Example input matrix
    float C[] = {1.0, 0.0};           // Example output matrix
    float D[] = {0.0};                // Example direct transmission matrix
    float x_init[] = {0.0, 0.0};      // Initial state

    init_state_space(&sys, x_init, A, B, C, D, 2, 1, 1);
    print_state_space(&sys);

    float A3[] = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0}; // Example state matrix
    float B3[] = {0.0, 1.0, 2.0};           // Example input matrix
    float C3[] = {1.0, 0.0, 0.0};           // Example output matrix
    float D3[] = {0.0};                // Example direct transmission matrix
    float x_init3[] = {0.0, 0.0, 0.0};      // Initial state

    init_state_space(&sys, x_init3, A3, B3, C3, D3, 3, 1, 1);
    print_state_space(&sys);

    return;
}