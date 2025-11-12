//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 100

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        
        char* endptr;
        long temp = strtol(buffer, &endptr, 10);
        
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (temp < min || temp > max) {
            printf("Value must be between %d and %d.\n", min, max);
            continue;
        }
        
        value = (int)temp;
        break;
    }
    
    return value;
}

void fill_matrix(int* matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Enter element [%d][%d]: ", i, j);
            int value = read_int("", INT_MIN, INT_MAX);
            *(matrix + i * cols + j) = value;
        }
    }
}

void print_matrix(int* matrix, int rows, int cols) {
    printf("Matrix (%dx%d):\n", rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d ", *(matrix + i * cols + j));
        }
        printf("\n");
    }
}

void multiply_matrices(int* a, int a_rows, int a_cols, int* b, int b_rows, int b_cols, int* result) {
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            int sum = 0;
            for (int k = 0; k < a_cols; k++) {
                sum += *(a + i * a_cols + k) * *(b + k * b_cols + j);
            }
            *(result + i * b_cols + j) = sum;
        }
    }
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    
    printf("Matrix Multiplication using Pointer Arithmetic\n");
    printf("=============================================\n\n");
    
    rows1 = read_int("Enter rows for matrix 1 (1-100): ", 1, MAX_DIM);
    if (rows1 == -1) return 1;
    
    cols1 = read_int("Enter columns for matrix 1 (1-100): ", 1, MAX_DIM);
    if (cols1 == -1) return 1;
    
    rows2 = read_int("Enter rows for matrix 2 (1-100): ", 1, MAX_DIM);
    if (rows2 == -1) return 1;
    
    cols2 = read_int("Enter columns for matrix 2 (1-100): ", 1, MAX_DIM);
    if (cols2 == -1) return 1;
    
    if (cols1 != rows2) {
        printf("Error: Matrix dimensions incompatible for multiplication.\n");
        printf("Columns of first matrix (%d) must equal rows of second matrix (%d).\n", cols1, rows2);
        return 1;
    }
    
    size_t size1 = (size_t)rows1 * cols1;
    size_t size2 = (size_t)rows2 * cols2;
    size_t result_size = (size_t)rows1 * cols2;
    
    if (size1 > SIZE_MAX / sizeof(int) || size2 > SIZE_MAX / sizeof(int) || 
        result_size > SIZE_MAX / sizeof(int)) {
        printf("Error: Matrix dimensions too large.\n");
        return 1;
    }
    
    int* matrix1 = malloc(size1 * sizeof(int));
    int* matrix2 = malloc(size2 * sizeof(int));
    int* result = malloc(result_size * sizeof(int));
    
    if (matrix1 == NULL || matrix2 == NULL || result == NULL) {
        printf("Error: Memory allocation failed.\n");
        free(matrix1);
        free(matrix2);
        free(result);
        return 1;
    }
    
    printf("\nEnter elements for matrix 1:\n");
    fill_matrix(matrix1, rows1, cols1);
    
    printf("\nEnter elements for matrix 2:\n");
    fill_matrix(matrix2, rows2, cols2);
    
    printf("\nMatrix 1:\n");
    print_matrix(matrix1, rows1, cols1);
    
    printf("\nMatrix 2:\n");
    print_matrix(matrix2, rows2, cols2);
    
    multiply_matrices(matrix1, rows1, cols1, matrix2, rows2, cols2, result);
    
    printf("\nResult of multiplication:\n");
    print_matrix(result, rows1, cols2);
    
    free(matrix1);
    free(matrix2);
    free(result);
    
    return 0;
}