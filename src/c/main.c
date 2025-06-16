// #include "example_state_space.h"
#include "lib/parseJSON/examples/example_parseJSON.h"

void main(void) {
    // example_state_space();
    example_parseJSON();
    // BUG: The matrix A is not being printed correctly.
    // This is because the built matrix A is out of scope
    // printing an uninitialized matrix.
    test_print_matrix(&A[0][0], 2 * MAX_DOF, 2 * MAX_DOF);
    return;
}
