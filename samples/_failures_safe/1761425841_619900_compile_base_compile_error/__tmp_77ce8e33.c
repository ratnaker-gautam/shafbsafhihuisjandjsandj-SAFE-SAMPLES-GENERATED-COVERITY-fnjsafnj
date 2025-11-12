//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10
#define MATRIX_IDX(mat, row, col, cols) ((mat)[(row) * (cols) + (col)])

void print_matrix(int *matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", MATRIX_IDX(matrix, i, j, cols));
        }
        printf("\n");
    }
}

int read_int_safe(const char *prompt, int min_val, int max_val) {
    char buffer[32];
    long val;
    char *endptr;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        val = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (val < min_val || val > max_val) {
            printf("Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        
        return (int)val;
    }
}

void matrix_multiply(int *a, int a_rows, int a_cols, int *b, int b_rows, int b_cols, int *result) {
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            int sum = 0;
            for (int k = 0; k < a_cols; k++) {
                sum += MATRIX_IDX(a, i, k, a_cols) * MATRIX_IDX(b, k, j, b_cols);
            }
            MATRIX_IDX(result, i, j, b_cols) = sum;
        }
    }
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    
    printf("Matrix Multiplication Program\n");
    printf("Matrix dimensions must be between 1 and %d\n", MAX_DIM);
    
    rows1 = read_int_safe("Enter rows for matrix 1: ", 1, MAX_DIM);
    cols1 = read_int_safe("Enter columns for matrix 1: ", 1, MAX_DIM);
    rows2 = read_int_safe("Enter rows for matrix 2: ", 1, MAX_DIM);
    cols2 = read_int_safe("Enter columns for matrix 2: ", 1, MAX_DIM);
    
    if (rows1 == -1 || cols1 == -1 || rows2 == -1 || cols2 == -1) {
        return EXIT_FAILURE;
    }
    
    if (cols1 != rows2) {
        printf("Error: Matrix dimensions incompatible for multiplication.\n");
        printf("Columns of first matrix (%d) must equal rows of second matrix (%d).\n", cols1, rows2);
        return EXIT_FAILURE;
    }
    
    size_t size1 = (size_t)rows1 * (size_t)cols1;
    size_t size2 = (size_t)rows2 * (size_t)cols2;
    size_t result_size = (size_t)rows1 * (size_t)cols2;
    
    if (size1 > SIZE_MAX / sizeof(int) || size2 > SIZE_MAX / sizeof(int) || 
        result_size > SIZE_MAX / sizeof(int)) {
        printf("Error: Matrix dimensions too large.\n");
        return EXIT_FAILURE;
    }
    
    int *matrix1 = malloc(size1 * sizeof(int));
    int *matrix2 = malloc(size2 * sizeof(int));
    int *result = malloc(result_size * sizeof(int));
    
    if (matrix1 == NULL || matrix2 == NULL || result == NULL) {
        printf("Memory allocation failed.\n");
        free(matrix1);
        free(matrix2);
        free(result);
        return EXIT_FAILURE;
    }
    
    printf("\nEnter elements for matrix 1 (%d x %d):\n", rows1, cols1);
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols1; j++) {
            printf("Element [%d][%d]: ", i, j);
            MATRIX_IDX(matrix1, i, j, cols1) = read_int_safe("", INT_MIN, INT_MAX);
            if (MATRIX_IDX(matrix1, i, j, cols1) == -1) {
                free(matrix1);
                free(matrix2);
                free(result);
                return EXIT_FAILURE;
            }
        }
    }
    
    printf("\nEnter elements for matrix 2 (%d x %d):\n", rows2, cols2);
    for (int i = 0; i < rows2; i++) {
        for (int j = 0; j < cols2; j++) {
            printf("Element [%d][%d]: ", i, j);
            MATRIX_IDX(matrix2, i, j, cols2) = read_int_safe("", INT_MIN, INT_MAX);
            if (MATRIX_IDX(matrix2, i, j, cols2) == -1) {