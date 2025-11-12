//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: matrix_ops
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
            return -1;
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

void multiply_matrices(int* a, int* b, int* result, int a_rows, int a_cols, int b_cols) {
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            int sum = 0;
            for (int k = 0; k < a_cols; k++) {
                sum += MATRIX_ELEM(a, i, k, a_cols) * MATRIX_ELEM(b, k, j, b_cols);
            }
            MATRIX_ELEM(result, i, j, b_cols) = sum;
        }
    }
}

int main() {
    int a_rows, a_cols, b_rows, b_cols;
    
    printf("Matrix A dimensions:\n");
    a_rows = read_int("Rows: ", 1, MAX_DIM);
    a_cols = read_int("Columns: ", 1, MAX_DIM);
    
    printf("Matrix B dimensions:\n");
    b_rows = read_int("Rows: ", 1, MAX_DIM);
    b_cols = read_int("Columns: ", 1, MAX_DIM);
    
    if (a_cols != b_rows) {
        printf("Matrix multiplication not possible: A columns (%d) must equal B rows (%d).\n", a_cols, b_rows);
        return 1;
    }
    
    int matrix_a[MAX_DIM * MAX_DIM];
    int matrix_b[MAX_DIM * MAX_DIM];
    int result[MAX_DIM * MAX_DIM];
    
    printf("Enter elements for Matrix A:\n");
    read_matrix(matrix_a, a_rows, a_cols);
    
    printf("Enter elements for Matrix B:\n");
    read_matrix(matrix_b, b_rows, b_cols);
    
    memset(result, 0, sizeof(result));
    multiply_matrices(matrix_a, matrix_b, result, a_rows, a_cols, b_cols);
    
    printf("Matrix A:\n");
    print_matrix(matrix_a, a_rows, a_cols);
    
    printf("Matrix B:\n");
    print_matrix(matrix_b, b_rows, b_cols);
    
    printf("Result (A * B):\n");
    print_matrix(result, a_rows, b_cols);
    
    return 0;
}