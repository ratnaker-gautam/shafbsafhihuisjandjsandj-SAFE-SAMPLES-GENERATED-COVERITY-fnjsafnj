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
    int rows1, cols1, rows2, cols2;
    
    printf("Matrix Multiplication Program\n");
    
    rows1 = read_int("Enter rows for matrix 1 (1-10): ", 1, MAX_DIM);
    if (rows1 == -1) return 1;
    cols1 = read_int("Enter columns for matrix 1 (1-10): ", 1, MAX_DIM);
    if (cols1 == -1) return 1;
    
    rows2 = read_int("Enter rows for matrix 2 (1-10): ", 1, MAX_DIM);
    if (rows2 == -1) return 1;
    cols2 = read_int("Enter columns for matrix 2 (1-10): ", 1, MAX_DIM);
    if (cols2 == -1) return 1;
    
    if (cols1 != rows2) {
        printf("Matrix multiplication not possible: columns of matrix 1 must equal rows of matrix 2.\n");
        return 1;
    }
    
    int matrix1[MAX_DIM * MAX_DIM];
    int matrix2[MAX_DIM * MAX_DIM];
    int result[MAX_DIM * MAX_DIM];
    
    printf("\nEnter elements for matrix 1:\n");
    read_matrix(matrix1, rows1, cols1);
    
    printf("\nEnter elements for matrix 2:\n");
    read_matrix(matrix2, rows2, cols2);
    
    memset(result, 0, sizeof(result));
    multiply_matrices(matrix1, matrix2, result, rows1, cols1, cols2);
    
    printf("\nMatrix 1:\n");
    print_matrix(matrix1, rows1, cols1);
    
    printf("\nMatrix 2:\n");
    print_matrix(matrix2, rows2, cols2);
    
    printf("\nResult of multiplication:\n");
    print_matrix(result, rows1, cols2);
    
    return 0;
}