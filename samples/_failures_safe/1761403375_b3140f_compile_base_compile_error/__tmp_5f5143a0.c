//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MATRIX_MAX_SIZE 10
#define MATRIX_MIN_SIZE 1
#define MATRIX_ELEM_MAX 1000
#define MATRIX_ELEM_MIN -1000

#define MATRIX_GET(m, r, c, cols) ((m)[(r) * (cols) + (c)])
#define MATRIX_SET(m, r, c, cols, val) ((m)[(r) * (cols) + (c)] = (val))

int read_int(const char* prompt, int min_val, int max_val) {
    char buffer[32];
    long result;
    char* endptr;
    
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min_val;
        }
        
        result = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\n') {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        
        if (result < min_val || result > max_val) {
            printf("Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        
        return (int)result;
    }
}

void fill_matrix(int* matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            MATRIX_SET(matrix, i, j, cols, 
                read_int("Enter matrix element: ", MATRIX_ELEM_MIN, MATRIX_ELEM_MAX));
        }
    }
}

void print_matrix(const int* matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", MATRIX_GET(matrix, i, j, cols));
        }
        printf("\n");
    }
}

void multiply_matrices(const int* a, const int* b, int* result, 
                      int a_rows, int a_cols, int b_cols) {
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            int sum = 0;
            for (int k = 0; k < a_cols; k++) {
                long product = (long)MATRIX_GET(a, i, k, a_cols) * 
                              (long)MATRIX_GET(b, k, j, b_cols);
                if (product > INT_MAX || product < INT_MIN) {
                    sum = 0;
                    break;
                }
                sum += (int)product;
                if ((sum > 0 && product < 0) || (sum < 0 && product > 0)) {
                    if (sum > 0) sum = INT_MAX;
                    else sum = INT_MIN;
                    break;
                }
            }
            MATRIX_SET(result, i, j, b_cols, sum);
        }
    }
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    
    printf("Matrix Multiplication Program\n");
    
    rows1 = read_int("Enter rows for matrix 1: ", MATRIX_MIN_SIZE, MATRIX_MAX_SIZE);
    cols1 = read_int("Enter columns for matrix 1: ", MATRIX_MIN_SIZE, MATRIX_MAX_SIZE);
    rows2 = read_int("Enter rows for matrix 2: ", MATRIX_MIN_SIZE, MATRIX_MAX_SIZE);
    cols2 = read_int("Enter columns for matrix 2: ", MATRIX_MIN_SIZE, MATRIX_MAX_SIZE);
    
    if (cols1 != rows2) {
        printf("Matrix dimensions incompatible for multiplication.\n");
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
    
    printf("\nEnter elements for matrix 1 (%dx%d):\n", rows1, cols1);
    fill_matrix(matrix1, rows1, cols1);
    
    printf("\nEnter elements for matrix 2 (%dx%d):\n", rows2, cols2);
    fill_matrix(matrix2, rows2, cols2);
    
    multiply_matrices(matrix1, matrix2, result, rows1, cols1, cols2);
    
    printf("\nMatrix 1:\n");
    print_matrix(matrix1, rows1, cols1);