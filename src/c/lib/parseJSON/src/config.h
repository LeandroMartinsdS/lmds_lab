#ifndef CONFIG_H
#define CONFIG_H

// Define matrix size (for simplicity, assuming a maximum of 4 DoFs)
#define MAX_DOF 4
#define STATE_PER_DOF 2 // Each DoF has a position and velocity state

// Define the maximum number of states
#define MAX_STATES (MAX_DOF * STATE_PER_DOF)
#define MAX_INPUTS 1 // Assuming a single input for the system

// Define state-space matrices
extern double A[MAX_STATES][MAX_STATES];
extern double B[MAX_STATES][MAX_INPUTS];
extern double C[MAX_INPUTS][MAX_STATES];
extern double D[MAX_INPUTS][MAX_INPUTS];

#endif // CONFIG_H