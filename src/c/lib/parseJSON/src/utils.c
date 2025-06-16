#include "utils.h"


// Function to read the entire contents of a file into a string
char *read_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("fopen");
        return NULL;
    }

    // Get the file size
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (length < 0) {
        perror("ftell");
        fclose(file);
        return NULL;
    }

    // Allocate memory for the file contents
    char *buffer = (char *)malloc(length + 1);
    if (!buffer) {
        perror("malloc");
        fclose(file);
        return NULL;
    }

    // Read the file contents into the buffer
    fread(buffer, 1, length, file);
    if (ferror(file)) {
        perror("fread");
        fclose(file);
        free(buffer);
        return NULL;
    }

    buffer[length] = '\0'; // Null-terminate the string

    fclose(file);
    return buffer;
}


void test_print_matrix(double *matrix, int rows, int cols) {
    int i = 0;
    for (; i < rows; i++) {
        int j = 0;
        for (; j < cols; j++) {
            printf("%6.5f ", *(matrix+i*cols+j));
        }
        printf("\n");
    }
}