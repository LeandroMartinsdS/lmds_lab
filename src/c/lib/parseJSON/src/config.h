#ifndef CONFIG_H
#define CONFIG_H

// Define matrix size (for simplicity, assuming a maximum of 4 DoFs)
#define MAX_DOF 4

// Define state-space matrices
extern double A[2 * MAX_DOF][2 * MAX_DOF];
extern double B[2 * MAX_DOF][1];
extern double C[1][2 * MAX_DOF];
extern double D[1][1];

#endif // CONFIG_H