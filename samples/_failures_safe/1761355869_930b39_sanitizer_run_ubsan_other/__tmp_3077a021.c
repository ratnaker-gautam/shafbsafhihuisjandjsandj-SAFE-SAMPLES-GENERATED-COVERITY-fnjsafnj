//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10
#define MATRIX_ELEM(mat, i, j, cols) ((mat)[(i) * (cols) + (j)])

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
        }
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        if (value < min || value > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        return value;
    }
}

void read_matrix(int* mat, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            MATRIX_ELEM(mat, i, j, cols) = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(int* mat, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", MATRIX_ELEM(mat, i, j, cols));
        }
        printf("\n");
    }
}

void multiply_matrices(int* a, int* b, int* result, int rows_a, int cols_a, int cols_b) {
    for (int i = 0; i < rows_a; i++) {
        for (int j = 0; j < cols_b; j++) {
            int sum = 0;
            for (int k = 0; k < cols_a; k++) {
                sum += MATRIX_ELEM(a, i, k, cols_a) * MATRIX_ELEM(b, k, j, cols_b);
            }
            MATRIX_ELEM(result, i, j, cols_b) = sum;
        }
    }
}

int main(void) {
    int rows_a, cols_a, rows_b, cols_b;
    
    printf("Matrix A dimensions:\n");
    rows_a = read_int("Rows: ", 1, MAX_DIM);
    cols_a = read_int("Columns: ", 1, MAX_DIM);
    
    printf("Matrix B dimensions:\n");
    rows_b = read_int("Rows: ", 1, MAX_DIM);
    cols_b = read_int("Columns: ", 1, MAX_DIM);
    
    if (cols_a != rows_b) {
        printf("Matrix multiplication not possible: columns of A (%d) must equal rows of B (%d).\n", cols_a, rows_b);
        return 1;
    }
    
    int matrix_a[MAX_DIM * MAX_DIM];
    int matrix_b[MAX_DIM * MAX_DIM];
    int result[MAX_DIM * MAX_DIM];
    
    printf("Enter elements for Matrix A:\n");
    read_matrix(matrix_a, rows_a, cols_a);
    
    printf("Enter elements for Matrix B:\n");
    read_matrix(matrix_b, rows_b, cols_b);
    
    multiply_matrices(matrix_a, matrix_b, result, rows_a, cols_a, cols_b);
    
    printf("Matrix A:\n");
    print_matrix(matrix_a, rows_a, cols_a);
    
    printf("Matrix B:\n");
    print_matrix(matrix_b, rows_b, cols_b);
    
    printf("Result (A * B):\n");
    print_matrix(result, rows_a, cols_b);
    
    return 0;
}