#include "example_parseJSON.h"

double A[2 * MAX_DOF][2 * MAX_DOF] = {0};
double B[2 * MAX_DOF][1] = {0};
double C[1][2 * MAX_DOF] = {0};
double D[1][1] = {0};

int example_parseJSON(void)
{
    // char *json_string = read_file("../examples/2ndOrderSystem.json");
    char *json_string = read_file("src/c/lib/parseJSON/examples/matrix.json");
    // char *json_string = read_file("../examples/matrix.json");

    if (json_string == NULL) {
        fprintf(stderr, "Error reading JSON file.\n");
        return 1;
    }

    // Get JSON type
    // // if 'mass_spring_damper' or 'RLC', then build matrices
    // // if 'matrices' then parse the matrices from JSON
    parse_json(json_string);

    // // Free the allocated memory
    // free(json_string);

	// CloseLibrary();
	return 0;
}

