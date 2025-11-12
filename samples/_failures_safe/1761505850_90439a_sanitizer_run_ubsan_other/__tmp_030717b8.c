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

void fill_matrix(int* mat, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            MATRIX_ELEM(mat, i, j, cols) = read_int(prompt, INT_MIN, INT_MAX);
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
    
    printf("Matrix Multiplication Program\n");
    printf("Matrix 1 dimensions:\n");
    rows1 = read_int("Rows: ", 1, MAX_DIM);
    cols1 = read_int("Columns: ", 1, MAX_DIM);
    
    printf("Matrix 2 dimensions:\n");
    rows2 = read_int("Rows: ", 1, MAX_DIM);
    cols2 = read_int("Columns: ", 1, MAX_DIM);
    
    if (cols1 != rows2) {
        printf("Error: Matrix 1 columns (%d) must equal Matrix 2 rows (%d) for multiplication.\n", cols1, rows2);
        return 1;
    }
    
    int size1 = rows1 * cols1;
    int size2 = rows2 * cols2;
    int result_size = rows1 * cols2;
    
    if (size1 <= 0 || size2 <= 0 || result_size <= 0) {
        printf("Error: Invalid matrix dimensions.\n");
        return 1;
    }
    
    int* matrix1 = malloc((size_t)size1 * sizeof(int));
    int* matrix2 = malloc((size_t)size2 * sizeof(int));
    int* result = malloc((size_t)result_size * sizeof(int));
    
    if (matrix1 == NULL || matrix2 == NULL || result == NULL) {
        printf("Error: Memory allocation failed.\n");
        free(matrix1);
        free(matrix2);
        free(result);
        return 1;
    }
    
    printf("\nEnter values for Matrix 1 (%dx%d):\n", rows1, cols1);
    fill_matrix(matrix1, rows1, cols1);
    
    printf("\nEnter values for Matrix 2 (%dx%d):\n", rows2, cols2);
    fill_matrix(matrix2, rows2, cols2);
    
    multiply_matrices(matrix1, rows1, cols1, matrix2, rows2, cols2, result);
    
    printf("\nMatrix 1:\n");
    print_matrix(matrix1, rows1, cols1);
    
    printf("\nMatrix 2:\n");
    print_matrix(matrix2, rows2, cols2);
    
    printf("\nResult Matrix (%dx%d):\n", rows1, cols2);
    print_matrix(result, rows1, cols2);
    
    free(matrix1);
    free(matrix2);
    free(result);
    
    return 0;
}