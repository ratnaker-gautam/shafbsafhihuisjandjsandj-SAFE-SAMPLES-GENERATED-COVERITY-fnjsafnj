//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10
#define MATRIX_SIZE(r, c) ((r) * (c))
#define MATRIX_INDEX(m, r, c, cols) ((m)[(r) * (cols) + (c)])

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

void read_matrix(int* matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            MATRIX_INDEX(matrix, i, j, cols) = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(int* matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", MATRIX_INDEX(matrix, i, j, cols));
        }
        printf("\n");
    }
}

void multiply_matrices(int* a, int* b, int* result, int a_rows, int a_cols, int b_cols) {
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            int sum = 0;
            for (int k = 0; k < a_cols; k++) {
                sum += MATRIX_INDEX(a, i, k, a_cols) * MATRIX_INDEX(b, k, j, b_cols);
            }
            MATRIX_INDEX(result, i, j, b_cols) = sum;
        }
    }
}

int main() {
    int rows1, cols1, rows2, cols2;
    
    printf("Matrix Multiplication Program\n");
    printf("Matrix dimensions must be between 1 and %d\n", MAX_DIM);
    
    rows1 = read_int("Enter rows for matrix 1: ", 1, MAX_DIM);
    cols1 = read_int("Enter columns for matrix 1: ", 1, MAX_DIM);
    rows2 = read_int("Enter rows for matrix 2: ", 1, MAX_DIM);
    cols2 = read_int("Enter columns for matrix 2: ", 1, MAX_DIM);
    
    if (cols1 != rows2) {
        printf("Error: Matrix dimensions incompatible for multiplication.\n");
        printf("Columns of matrix 1 (%d) must equal rows of matrix 2 (%d).\n", cols1, rows2);
        return 1;
    }
    
    int matrix1[MATRIX_SIZE(rows1, cols1)];
    int matrix2[MATRIX_SIZE(rows2, cols2)];
    int result[MATRIX_SIZE(rows1, cols2)];
    
    printf("\nEnter values for matrix 1 (%d x %d):\n", rows1, cols1);
    read_matrix(matrix1, rows1, cols1);
    
    printf("\nEnter values for matrix 2 (%d x %d):\n", rows2, cols2);
    read_matrix(matrix2, rows2, cols2);
    
    printf("\nMatrix 1:\n");
    print_matrix(matrix1, rows1, cols1);
    
    printf("\nMatrix 2:\n");
    print_matrix(matrix2, rows2, cols2);
    
    multiply_matrices(matrix1, matrix2, result, rows1, cols1, cols2);
    
    printf("\nResult of multiplication (%d x %d):\n", rows1, cols2);
    print_matrix(result, rows1, cols2);
    
    return 0;
}