#include <stdio.h>
#include <string.h>
#include "../../cJSON/cJSON.h"
#include "config.h"
#include "parse_matrix.h"
#include "parse_ss.h"
// #include "config.h"

#ifndef PARSE_JSON_H
#define PARSE_JSON_H

// Function to read the entire contents of a file into a string
char *read_file(const char *filename);
void handle_json_type( cJSON *json_type, cJSON *json_str);
void parse_json(const char *json_string);

#endif // PARSE_JSON_H