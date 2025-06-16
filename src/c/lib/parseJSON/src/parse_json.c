#include "parse_json.h"

// Function to handle the JSON type and call the appropriate parsing function
void handle_json_type( cJSON *json_type, cJSON *json_str) {
    if (json_type == NULL) {
        printf("Error reading type\n");
        return;
    }
    if (!cJSON_IsString(json_type)) {
        printf("Error: 'json_type' is not a string\n");
        return;
    }

    if(strcmp(json_type->valuestring, "matrix") == 0) {
        printf("Parsing matrix type JSON\n");
        parse_matrixA(json_type, json_str);
        // parse_matrixB(json_type);
        // parse_matrixC(json_type);
        // parse_matrixD(json_type);
    } else if(strcmp(json_type->valuestring, "transfer_function") == 0) {
        // PLACEHOLDER
    } else if(strcmp(json_type->valuestring, "mass_spring_dampness") == 0) {
        // build_state_space(json_type);
     } else if(strcmp(json_type->valuestring, "RLC") == 0) {
        //PLACEHOLDER
    } else {
        printf("Error: invalid type\n");
    }
}


// Function to build the state-space representation from JSON data
// This function will be called from parse_json() after parsing the JSON data
// and extracting the relevant information.
void parse_json(const char *json_string) {
    cJSON *json = cJSON_Parse(json_string);
    if (json == NULL) {
        const char *error_ptr = cJSON_GetErrorPtr();
        printf("Error parsing JSON\n");
        if (error_ptr != NULL) {
            printf("%s\n", error_ptr);
        }
        return;
    }

    cJSON *type = cJSON_GetObjectItem(json, "type");
    handle_json_type(type, json);

    cJSON_Delete(json);
}
