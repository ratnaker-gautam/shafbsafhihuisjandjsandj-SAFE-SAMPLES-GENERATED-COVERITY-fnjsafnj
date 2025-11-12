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
        if (sscanf(buffer, "%d", &value) == 1 && value >= min && value <= max) {
            return value;
        }
        printf("Invalid input. Enter integer between %d and %d.\n", min, max);
    }
}

void fill_matrix(int* mat, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            MATRIX_ELEM(mat, i, j, cols) = read_int("Enter element: ", INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(const int* mat, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", MATRIX_ELEM(mat, i, j, cols));
        }
        printf("\n");
    }
}

void multiply_matrices(const int* a, int a_rows, int a_cols,
                      const int* b, int b_rows, int b_cols,
                      int* result) {
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

int main(void) {
    int rows1, cols1, rows2, cols2;
    
    printf("Matrix Multiplication\n");
    
    rows1 = read_int("Enter rows for matrix 1: ", 1, MAX_DIM);
    cols1 = read_int("Enter columns for matrix 1: ", 1, MAX_DIM);
    
    rows2 = read_int("Enter rows for matrix 2: ", 1, MAX_DIM);
    cols2 = read_int("Enter columns for matrix 2: ", 1, MAX_DIM);
    
    if (cols1 != rows2) {
        printf("Matrix dimensions incompatible for multiplication.\n");
        return 1;
    }
    
    int matrix1[MAX_DIM * MAX_DIM];
    int matrix2[MAX_DIM * MAX_DIM];
    int result[MAX_DIM * MAX_DIM];
    
    printf("Enter elements for matrix 1:\n");
    fill_matrix(matrix1, rows1, cols1);
    
    printf("Enter elements for matrix 2:\n");
    fill_matrix(matrix2, rows2, cols2);
    
    memset(result, 0, sizeof(result));
    multiply_matrices(matrix1, rows1, cols1, matrix2, rows2, cols2, result);
    
    printf("Matrix 1:\n");
    print_matrix(matrix1, rows1, cols1);
    
    printf("Matrix 2:\n");
    print_matrix(matrix2, rows2, cols2);
    
    printf("Result:\n");
    print_matrix(result, rows1, cols2);
    
    return 0;
}