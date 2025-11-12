//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10
#define MATRIX_ELEMENT_MAX 1000
#define MATRIX_ELEMENT_MIN -1000

#define MATRIX_OP_ADD 1
#define MATRIX_OP_SUBTRACT 2
#define MATRIX_OP_MULTIPLY 3

#define SAFE_MALLOC(size, ptr) do { \
    ptr = malloc(size); \
    if (!ptr) { \
        fprintf(stderr, "Memory allocation failed\n"); \
        exit(EXIT_FAILURE); \
    } \
} while(0)

#define VALIDATE_SIZE(n) do { \
    if (n <= 0 || n > MAX_SIZE) { \
        fprintf(stderr, "Invalid matrix size\n"); \
        exit(EXIT_FAILURE); \
    } \
} while(0)

#define VALIDATE_ELEMENT(val) do { \
    if (val < MATRIX_ELEMENT_MIN || val > MATRIX_ELEMENT_MAX) { \
        fprintf(stderr, "Matrix element out of range\n"); \
        exit(EXIT_FAILURE); \
    } \
} while(0)

void read_matrix(int **matrix, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int value;
            if (scanf("%d", &value) != 1) {
                fprintf(stderr, "Invalid input\n");
                exit(EXIT_FAILURE);
            }
            VALIDATE_ELEMENT(value);
            matrix[i][j] = value;
        }
    }
}

void print_matrix(int **matrix, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int **allocate_matrix(int size) {
    int **matrix;
    SAFE_MALLOC(size * sizeof(int *), matrix);
    for (int i = 0; i < size; i++) {
        SAFE_MALLOC(size * sizeof(int), matrix[i]);
        memset(matrix[i], 0, size * sizeof(int));
    }
    return matrix;
}

void free_matrix(int **matrix, int size) {
    for (int i = 0; i < size; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

int **add_matrices(int **a, int **b, int size) {
    int **result = allocate_matrix(size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            long long sum = (long long)a[i][j] + b[i][j];
            if (sum > INT_MAX || sum < INT_MIN) {
                fprintf(stderr, "Integer overflow in addition\n");
                free_matrix(result, size);
                exit(EXIT_FAILURE);
            }
            result[i][j] = (int)sum;
            VALIDATE_ELEMENT(result[i][j]);
        }
    }
    return result;
}

int **subtract_matrices(int **a, int **b, int size) {
    int **result = allocate_matrix(size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            long long diff = (long long)a[i][j] - b[i][j];
            if (diff > INT_MAX || diff < INT_MIN) {
                fprintf(stderr, "Integer overflow in subtraction\n");
                free_matrix(result, size);
                exit(EXIT_FAILURE);
            }
            result[i][j] = (int)diff;
            VALIDATE_ELEMENT(result[i][j]);
        }
    }
    return result;
}

int **multiply_matrices(int **a, int **b, int size) {
    int **result = allocate_matrix(size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            long long sum = 0;
            for (int k = 0; k < size; k++) {
                long long product = (long long)a[i][k] * b[k][j];
                if (product > INT_MAX || product < INT_MIN) {
                    fprintf(stderr, "Integer overflow in multiplication\n");
                    free_matrix(result, size);
                    exit(EXIT_FAILURE);
                }
                sum += product;
                if (sum > INT_MAX || sum < INT_MIN) {
                    fprintf(stderr, "Integer overflow in multiplication\n");
                    free_matrix(result, size);
                    exit(EXIT_FAILURE);
                }
            }
            result[i][j] = (int)sum;
            VALIDATE_ELEMENT(result[i][j]);
        }
    }
    return result;
}

int main() {
    int size;
    if (scanf("%d", &size) != 1) {
        fprintf(stderr, "Failed to read matrix size\n");
        return EXIT_FAILURE;
    }
    VALIDATE_SIZE(size);

    int operation;
    if (scanf("%d", &operation) != 1) {
        fprintf(stderr, "Failed to read operation\n");
        return EXIT_FAILURE;
    }
    if (operation < MATRIX_OP_ADD || operation > MATRIX_OP_MULTIPLY) {
        fprintf