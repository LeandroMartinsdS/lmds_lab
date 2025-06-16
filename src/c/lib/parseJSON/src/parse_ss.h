#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../cJSON/cJSON.h"
#include "config.h"

#ifndef PARSE_SS_H
#define PARSE_SS_H
// Function to read the entire contents of a file into a string
void build_state_space(cJSON *json_type);

#endif // PARSE_SS_H