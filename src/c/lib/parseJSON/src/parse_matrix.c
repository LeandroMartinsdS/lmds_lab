#include "parse_matrix.h"

int parse_matrixA(cJSON *json_type, cJSON *json_str) {
    int num_rows, num_columns;
    int i, j;

    cJSON *jsonA = cJSON_GetObjectItem(json_str, "A");
    if (jsonA == NULL) {
        printf("Error: 'A' not found\n");
        return 1;
    } else if (!cJSON_IsArray(jsonA)) {
        printf("Error: 'A' is not an array\n");
        return 1;
    }

    num_rows = cJSON_GetArraySize(jsonA);
    if (num_rows < 1 || num_rows > MAX_DOF){
        printf("Error: invalid number of rows\n");
        return 1;
    }
    num_columns = cJSON_GetArraySize(cJSON_GetArrayItem(jsonA, 0));
    if (num_columns < 1 || num_columns > MAX_DOF || num_columns != num_rows){
        printf("Error: invalid number of columns\n");
        return 1;
    }
    if (num_columns != num_rows){
        printf("Error: matrix A is not square\n");
        return 1;
    }

    for (i = 0; i < num_rows; i++) {
        cJSON *row = cJSON_GetArrayItem(jsonA, i);
        if (cJSON_GetArraySize(row) != num_columns) {
            printf("Error: row %d has the wrong number of columns\n", row->valueint);
            return 1;
        } else for (j = 0; j < num_columns; j++) {
            A[i][j]=cJSON_GetArrayItem(row, j)->valuedouble;
        }
    }
    return 0;
}
