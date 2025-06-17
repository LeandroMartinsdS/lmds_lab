#include "parse_ss.h"


void build_matrix_A(void) {
    // This function is a placeholder for building the A matrix.
}

void build_matrix_B(void) {
    // This function is a placeholder for building the B matrix.
}

void build_matrix_C(void) {
    // This function is a placeholder for building the C matrix.
}

void build_matrix_D(void) {
    // This function is a placeholder for building the D matrix.
}


void build_state_space(cJSON *json_str) {
    // TODO: Add another argument to choose in which
    // format the matrix will be built

    // Extract DoFs - TODO: move to another function
    cJSON *DoFs = cJSON_GetObjectItem(json_str, "DoFs");
    if (DoFs == NULL) {
        printf("Error: 'DoFs' not found\n");
        return;
    } else if (!cJSON_IsArray(DoFs)) {
        printf("Error: 'DoFs' is not an array\n");
        return;
    }

    int DoF_count = cJSON_GetArraySize(DoFs);
    int id;
    double mass[DoF_count], stiffness, damping;
    int i = 0;
    for (; i < DoF_count; i++) {
        cJSON *DoF = cJSON_GetArrayItem(DoFs, i);
        id = cJSON_GetObjectItem(DoF, "id")->valueint;
        // TO DO: Validate id
        mass[id] = cJSON_GetObjectItem(DoF, "mass")->valuedouble;
    }

    cJSON *connections = cJSON_GetObjectItem(json_str, "connections");
    // if (connections != NULL) {

    // }
    // if (!cJSON_IsArray(connections)) {

    // }
    int connections_count = cJSON_GetArraySize(connections);
    const char *id_source;
    const char *id_dest;

    for (i = 0; i < connections_count; i++) {
        // Extract connections
        cJSON *connection = cJSON_GetArrayItem(connections, i);
        id_source = cJSON_GetObjectItem(connection, "id_source")->valuestring;
        id_dest   = cJSON_GetObjectItem(connection, "id_dest")->valuestring;
        stiffness = cJSON_GetObjectItem(connection, "stiffness")->valuedouble;
        damping = cJSON_GetObjectItem(connection, "damping")->valuedouble;

        // DoF-wise mode: X^T=[x_1, xdot_1, x_2, xdot_2, ...]
        if (strcmp(id_source, "ground") == 0 && strcmp(id_dest, "ground") != 0) {
            // Update A matrix for ground connection
            A[2 * (atoi(id_dest) - 1)][2 * (atoi(id_dest) - 1) + 1] = 1;
            A[2 * (atoi(id_dest) - 1) + 1][2 * (atoi(id_dest) - 1)] = -stiffness / mass[atoi(id_dest)];
            A[2 * (atoi(id_dest) - 1) + 1][2 * (atoi(id_dest) - 1) + 1] = -damping / mass[atoi(id_dest)];

        } else if (strcmp(id_source, "ground") == 1 && strcmp(id_dest, "ground") != 0) {
            // Update A matrix for ground connection
            A[2 * (atoi(id_source) - 1)][2 * (atoi(id_source) - 1) + 1] = 1;
            A[2 * (atoi(id_source) - 1) + 1][2 * (atoi(id_source) - 1)]     = -stiffness / mass[atoi(id_source)];
            A[2 * (atoi(id_source) - 1) + 1][2 * (atoi(id_source) - 1) + 1] = -damping / mass[atoi(id_source)];

        } else if (atoi(id_source) > 0        && atoi(id_dest) > 0
                && atoi(id_source) <= MAX_DOF && atoi(id_dest) <= MAX_DOF)  {
            // Update A matrix for DoF connection
            A[2 * (atoi(id_dest) - 1)][2 * (atoi(id_dest) - 1) + 1] = 1;
            A[2 * (atoi(id_dest) - 1) + 1][2 * (atoi(id_dest) - 1)] -= stiffness / mass[atoi(id_dest)];
            A[2 * (atoi(id_dest) - 1) + 1][2 * (atoi(id_dest) - 1) + 1] -= damping / mass[atoi(id_dest)];
            A[2 * (atoi(id_dest) - 1) + 1][2 * (atoi(id_source) - 1)] += stiffness / mass[atoi(id_dest)];
            A[2 * (atoi(id_dest) - 1) + 1][2 * (atoi(id_source) - 1) + 1] += damping / mass[atoi(id_dest)];

            A[2 * (atoi(id_source) - 1)][2 * (atoi(id_source) - 1) + 1] = 1;
            A[2 * (atoi(id_source) - 1) + 1][2 * (atoi(id_source) - 1)]     -= stiffness / mass[atoi(id_source)];
            A[2 * (atoi(id_source) - 1) + 1][2 * (atoi(id_source) - 1) + 1] -= damping / mass[atoi(id_source)];
            A[2 * (atoi(id_source) - 1) + 1][2 * (atoi(id_dest) - 1)]     += stiffness / mass[atoi(id_source)];
            A[2 * (atoi(id_source) - 1) + 1][2 * (atoi(id_dest) - 1) + 1] += damping / mass[atoi(id_source)];
        }


        /////////////////////////////////////////////////////////////////////////////
        // Differential Order-wise mode: X^T=[x_1, x_2, ... , xdot_1, xdot_2, ...]
        // if (strcmp(id_source, "ground") == 0 && strcmp(id_dest, "ground") != 0) {
        //     // Update A matrix for ground connection
        //     A[(atoi(id_dest) - 1)][(atoi(id_dest) + DoF_count - 1)] = 1;
        //     A[(atoi(id_dest) + DoF_count - 1)][(atoi(id_dest) - 1)] = -stiffness / mass[atoi(id_dest)];
        //     A[(atoi(id_dest) + DoF_count - 1)][(atoi(id_dest) + DoF_count - 1)] = -damping / mass[atoi(id_dest)];

        // } else if (strcmp(id_source, "ground") == 1 && strcmp(id_dest, "ground") != 0) {
        //     // Update A matrix for ground connection
        //     A[(atoi(id_source) - 1)][(atoi(id_source) + DoF_count - 1)] = 1;
        //     A[(atoi(id_source) + DoF_count  - 1)][(atoi(id_source) - 1)]     = -stiffness / mass[atoi(id_source)];
        //     A[(atoi(id_source) + DoF_count  - 1)][(atoi(id_source) + DoF_count - 1)] = -damping / mass[atoi(id_source)];

        // } else if (atoi(id_source) > 0        && atoi(id_dest) > 0
        //         && atoi(id_source) <= MAX_DOF && atoi(id_dest) <= MAX_DOF)  {
        //     // Update A matrix for DoF connection
        //     A[(atoi(id_dest) - 1)][(atoi(id_dest) + DoF_count - 1)] = 1;
        //     A[(atoi(id_dest) + DoF_count - 1)][(atoi(id_dest) - 1)] -= stiffness / mass[atoi(id_dest)];
        //     A[(atoi(id_dest) + DoF_count - 1)][(atoi(id_dest) + DoF_count - 1)] -= damping / mass[atoi(id_dest)];
        //     A[(atoi(id_dest) + DoF_count - 1)][(atoi(id_source) - 1)] += stiffness / mass[atoi(id_dest)];
        //     A[(atoi(id_dest) + DoF_count - 1)][(atoi(id_source) + DoF_count - 1)] += damping / mass[atoi(id_dest)];

        //     A[(atoi(id_source) - 1)][(atoi(id_source) + DoF_count - 1)] = 1;
        //     A[(atoi(id_source) + DoF_count  - 1)][(atoi(id_source) - 1)]     -= stiffness / mass[atoi(id_source)];
        //     A[(atoi(id_source) + DoF_count  - 1)][(atoi(id_source) + DoF_count - 1)] -= damping / mass[atoi(id_source)];
        //     A[(atoi(id_source) + DoF_count  - 1)][(atoi(id_dest) - 1)]     += stiffness / mass[atoi(id_source)];
        //     A[(atoi(id_source) + DoF_count  - 1)][(atoi(id_dest) + DoF_count - 1)] += damping / mass[atoi(id_source)];
        // }
        /////////////////////////////////////////////////////////////////////////////
    }
}