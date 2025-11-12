//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 8

typedef void (*MatrixOp)(int rows, int cols, int matrix[rows][cols]);

void validate_matrix_size(int rows, int cols) {
    if (rows <= 0 || cols <= 0 || rows > MAX_SIZE || cols > MAX_SIZE) {
        fprintf(stderr, "Error: Matrix dimensions must be between 1 and %d\n", MAX_SIZE);
        exit(EXIT_FAILURE);
    }
}

void read_matrix(int rows, int cols, int matrix[rows][cols]) {
    printf("Enter %d x %d matrix elements:\n", rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char buffer[32];
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                fprintf(stderr, "Error reading input\n");
                exit(EXIT_FAILURE);
            }
            char *endptr;
            long val = strtol(buffer, &endptr, 10);
            if (endptr == buffer || *endptr != '\n' || val < INT_MIN || val > INT_MAX) {
                fprintf(stderr, "Invalid integer input\n");
                exit(EXIT_FAILURE);
            }
            matrix[i][j] = (int)val;
        }
    }
}

void print_matrix(int rows, int cols, int matrix[rows][cols]) {
    printf("Matrix %d x %d:\n", rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", matrix[i][j]);
        }
        printf("\n");
    }
}

void transpose_matrix(int rows, int cols, int matrix[rows][cols]) {
    if (rows != cols) {
        printf("Transpose requires square matrix\n");
        return;
    }
    for (int i = 0; i < rows; i++) {
        for (int j = i + 1; j < cols; j++) {
            int temp = matrix[i][j];
            matrix[i][j] = matrix[j][i];
            matrix[j][i] = temp;
        }
    }
}

void scale_matrix(int rows, int cols, int matrix[rows][cols]) {
    printf("Enter scaling factor: ");
    char buffer[32];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return;
    }
    char *endptr;
    long factor = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n' || factor < INT_MIN || factor > INT_MAX) {
        fprintf(stderr, "Invalid scaling factor\n");
        return;
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            long result = (long)matrix[i][j] * factor;
            if (result > INT_MAX || result < INT_MIN) {
                fprintf(stderr, "Scaling would cause integer overflow\n");
                return;
            }
            matrix[i][j] = (int)result;
        }
    }
}

void add_identity(int rows, int cols, int matrix[rows][cols]) {
    if (rows != cols) {
        printf("Identity addition requires square matrix\n");
        return;
    }
    for (int i = 0; i < rows; i++) {
        long result = (long)matrix[i][i] + 1;
        if (result > INT_MAX || result < INT_MIN) {
            fprintf(stderr, "Identity addition would cause integer overflow\n");
            return;
        }
        matrix[i][i] = (int)result;
    }
}

void negate_matrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (matrix[i][j] == INT_MIN) {
                fprintf(stderr, "Negation would cause integer overflow\n");
                return;
            }
            matrix[i][j] = -matrix[i][j];
        }
    }
}

void apply_operation(int rows, int cols, int matrix[rows][cols], MatrixOp operation) {
    operation(rows, cols, matrix);
}

int main(void) {
    int rows, cols;
    char buffer[32];
    
    printf("Enter number of rows (1-%d): ", MAX_SIZE);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    char *endptr;
    long r = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n' || r <= 0 || r > MAX_SIZE) {
        fprintf(stderr, "Invalid row count\n");
        return EXIT_FAILURE;
    }
    rows = (int)r;
    
    printf("Enter number of columns (1-%d): ", MAX_SIZE);
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    long c = strtol(buffer, &endptr,