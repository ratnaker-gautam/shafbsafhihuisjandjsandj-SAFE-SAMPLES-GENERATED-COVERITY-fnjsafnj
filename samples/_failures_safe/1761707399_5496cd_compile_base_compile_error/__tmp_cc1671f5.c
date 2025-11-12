//DeepSeek-V3 V2.5 Category: Safe ; Style: function_pointers ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

typedef void (*MatrixOp)(int rows, int cols, int matrix[rows][cols]);

void validate_matrix_size(int rows, int cols) {
    if (rows <= 0 || rows > MAX_SIZE || cols <= 0 || cols > MAX_SIZE) {
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
                fprintf(stderr, "Error: Invalid integer input\n");
                exit(EXIT_FAILURE);
            }
            matrix[i][j] = (int)val;
        }
    }
}

void print_matrix(int rows, int cols, int matrix[rows][cols]) {
    printf("Matrix:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", matrix[i][j]);
        }
        printf("\n");
    }
}

void transpose_matrix(int rows, int cols, int matrix[rows][cols]) {
    if (rows != cols) {
        printf("Cannot transpose non-square matrix\n");
        return;
    }
    for (int i = 0; i < rows; i++) {
        for (int j = i + 1; j < cols; j++) {
            int temp = matrix[i][j];
            matrix[i][j] = matrix[j][i];
            matrix[j][i] = temp;
        }
    }
    printf("Matrix transposed\n");
}

void scale_matrix(int rows, int cols, int matrix[rows][cols]) {
    printf("Enter scaling factor: ");
    char buffer[32];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        exit(EXIT_FAILURE);
    }
    char *endptr;
    long factor = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n' || factor < INT_MIN || factor > INT_MAX) {
        fprintf(stderr, "Error: Invalid scaling factor\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            long result = (long)matrix[i][j] * factor;
            if (result > INT_MAX || result < INT_MIN) {
                fprintf(stderr, "Error: Scaling would cause integer overflow\n");
                exit(EXIT_FAILURE);
            }
            matrix[i][j] = (int)result;
        }
    }
    printf("Matrix scaled by %ld\n", factor);
}

void add_constant(int rows, int cols, int matrix[rows][cols]) {
    printf("Enter constant to add: ");
    char buffer[32];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        exit(EXIT_FAILURE);
    }
    char *endptr;
    long constant = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n' || constant < INT_MIN || constant > INT_MAX) {
        fprintf(stderr, "Error: Invalid constant\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            long result = (long)matrix[i][j] + constant;
            if (result > INT_MAX || result < INT_MIN) {
                fprintf(stderr, "Error: Addition would cause integer overflow\n");
                exit(EXIT_FAILURE);
            }
            matrix[i][j] = (int)result;
        }
    }
    printf("Constant %ld added to matrix\n", constant);
}

void execute_operation(MatrixOp op, int rows, int cols, int matrix[rows][cols]) {
    op(rows, cols, matrix);
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
        fprintf(stderr, "Error: Invalid row count\n");
        return EXIT_FAILURE;
    }
    rows = (int