#include <stdio.h>
#include "../../cJSON/cJSON.h"
#include "config.h"


#ifndef PARSE_MATRIX_H
#define PARSE_MATRIX_H
// Function to read the entire contents of a file into a string
int parse_matrixA(cJSON *json_type, cJSON *json_str);
int parse_matrixB(cJSON *json_type);
int parse_matrixC(cJSON *json_type);
int parse_matrixD(cJSON *json_type);
#endif // PARSE_MATRIX_H