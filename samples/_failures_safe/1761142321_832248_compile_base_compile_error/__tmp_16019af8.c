//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10
#define MATRIX_ELEMENT_MAX 1000
#define MATRIX_ELEMENT_MIN -1000

#define MATRIX_GET(m, r, c, cols) ((m)[(r) * (cols) + (c)])
#define MATRIX_SET(m, r, c, cols, val) ((m)[(r) * (cols) + (c)] = (val))

int read_int(const char* prompt, int min_val, int max_val) {
    char buffer[32];
    long value;
    char* endptr;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min_val;
        }
        
        value = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (value < min_val || value > max_val) {
            printf("Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        
        return (int)value;
    }
}

void read_matrix(int* matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), 
                    "Enter element [%d][%d]: ", i, j);
            MATRIX_SET(matrix, i, j, cols, 
                      read_int(prompt, MATRIX_ELEMENT_MIN, MATRIX_ELEMENT_MAX));
        }
    }
}

void print_matrix(int* matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", MATRIX_GET(matrix, i, j, cols));
        }
        printf("\n");
    }
}

void multiply_matrices(int* a, int* b, int* result, 
                      int a_rows, int a_cols, int b_cols) {
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            int sum = 0;
            for (int k = 0; k < a_cols; k++) {
                long product = (long)MATRIX_GET(a, i, k, a_cols) * 
                              (long)MATRIX_GET(b, k, j, b_cols);
                if (product > INT_MAX || product < INT_MIN) {
                    printf("Integer overflow detected.\n");
                    exit(1);
                }
                sum += (int)product;
                if ((sum > 0 && product > 0 && sum < 0) || 
                    (sum < 0 && product < 0 && sum > 0)) {
                    printf("Integer overflow detected.\n");
                    exit(1);
                }
            }
            MATRIX_SET(result, i, j, b_cols, sum);
        }
    }
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    
    printf("Matrix Multiplication Program\n");
    printf("Matrix size limit: %dx%d\n", MAX_SIZE, MAX_SIZE);
    
    rows1 = read_int("Enter rows for matrix 1: ", 1, MAX_SIZE);
    cols1 = read_int("Enter columns for matrix 1: ", 1, MAX_SIZE);
    rows2 = read_int("Enter rows for matrix 2: ", 1, MAX_SIZE);
    cols2 = read_int("Enter columns for matrix 2: ", 1, MAX_SIZE);
    
    if (cols1 != rows2) {
        printf("Matrix dimensions incompatible for multiplication.\n");
        printf("Columns of first matrix (%d) must equal rows of second matrix (%d).\n", 
               cols1, rows2);
        return 1;
    }
    
    size_t size1 = (size_t)rows1 * (size_t)cols1;
    size_t size2 = (size_t)rows2 * (size_t)cols2;
    size_t result_size = (size_t)rows1 * (size_t)cols2;
    
    if (size1 > SIZE_MAX / sizeof(int) || 
        size2 > SIZE_MAX / sizeof(int) || 
        result_size > SIZE_MAX / sizeof(int)) {
        printf("Matrix size too large.\n");
        return 1;
    }
    
    int* matrix1 = malloc(size1 * sizeof(int));
    int* matrix2 = malloc(size2 * sizeof(int));
    int* result = malloc(result_size * sizeof(int));
    
    if (matrix1 == NULL || matrix2 == NULL || result == NULL) {
        printf("Memory allocation failed.\n");
        free(matrix1);
        free(matrix2);
        free(result);
        return 1;
    }
    
    printf("\nEnter elements for matrix 1:\n");
    read_matrix(matrix1, rows1, cols1);
    
    printf("\nEnter elements for matrix 2:\n");
    read_matrix(matrix2, rows2, cols2);
    
    printf("\nMatrix 1:\n");
    print_matrix(matrix1, rows1, cols1);