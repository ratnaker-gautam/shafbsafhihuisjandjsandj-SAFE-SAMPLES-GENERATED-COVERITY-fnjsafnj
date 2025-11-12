//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10
#define MATRIX_SIZE (MAX_DIM * MAX_DIM)
#define INPUT_BUFFER_SIZE 256

#define MATRIX_GET(m, r, c, cols) ((m)[(r) * (cols) + (c)])
#define MATRIX_SET(m, r, c, cols, val) ((m)[(r) * (cols) + (c)] = (val))

int read_int_safe(int *value) {
    char buffer[INPUT_BUFFER_SIZE];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    char *endptr;
    long val = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') {
        return 0;
    }
    if (val < INT_MIN || val > INT_MAX) {
        return 0;
    }
    *value = (int)val;
    return 1;
}

int read_matrix(int *matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Enter element [%d][%d]: ", i, j);
            int value;
            if (!read_int_safe(&value)) {
                return 0;
            }
            MATRIX_SET(matrix, i, j, cols, value);
        }
    }
    return 1;
}

void print_matrix(int *matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", MATRIX_GET(matrix, i, j, cols));
        }
        printf("\n");
    }
}

int multiply_matrices(int *a, int a_rows, int a_cols, int *b, int b_rows, int b_cols, int *result) {
    if (a_cols != b_rows) {
        return 0;
    }
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            int sum = 0;
            for (int k = 0; k < a_cols; k++) {
                long product = (long)MATRIX_GET(a, i, k, a_cols) * (long)MATRIX_GET(b, k, j, b_cols);
                if (product > INT_MAX || product < INT_MIN) {
                    return 0;
                }
                long new_sum = (long)sum + product;
                if (new_sum > INT_MAX || new_sum < INT_MIN) {
                    return 0;
                }
                sum = (int)new_sum;
            }
            MATRIX_SET(result, i, j, b_cols, sum);
        }
    }
    return 1;
}

int transpose_matrix(int *matrix, int rows, int cols, int *result) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            MATRIX_SET(result, j, i, rows, MATRIX_GET(matrix, i, j, cols));
        }
    }
    return 1;
}

int main() {
    int matrix_a[MATRIX_SIZE];
    int matrix_b[MATRIX_SIZE];
    int result[MATRIX_SIZE];
    
    int rows_a, cols_a, rows_b, cols_b;
    
    printf("Enter dimensions for matrix A (rows columns): ");
    if (!read_int_safe(&rows_a) || !read_int_safe(&cols_a)) {
        printf("Invalid input for matrix A dimensions.\n");
        return 1;
    }
    if (rows_a <= 0 || rows_a > MAX_DIM || cols_a <= 0 || cols_a > MAX_DIM) {
        printf("Matrix A dimensions out of bounds.\n");
        return 1;
    }
    
    printf("Enter dimensions for matrix B (rows columns): ");
    if (!read_int_safe(&rows_b) || !read_int_safe(&cols_b)) {
        printf("Invalid input for matrix B dimensions.\n");
        return 1;
    }
    if (rows_b <= 0 || rows_b > MAX_DIM || cols_b <= 0 || cols_b > MAX_DIM) {
        printf("Matrix B dimensions out of bounds.\n");
        return 1;
    }
    
    printf("Enter elements for matrix A:\n");
    if (!read_matrix(matrix_a, rows_a, cols_a)) {
        printf("Invalid input for matrix A elements.\n");
        return 1;
    }
    
    printf("Enter elements for matrix B:\n");
    if (!read_matrix(matrix_b, rows_b, cols_b)) {
        printf("Invalid input for matrix B elements.\n");
        return 1;
    }
    
    printf("\nMatrix A:\n");
    print_matrix(matrix_a, rows_a, cols_a);
    
    printf("\nMatrix B:\n");
    print_matrix(matrix_b, rows_b, cols_b);
    
    printf("\nMatrix A transpose:\n");
    int transpose_a[MATRIX_SIZE];
    transpose_matrix(matrix_a, rows_a, cols_a, transpose_a);
    print_matrix(transpose_a, cols_a, rows_a);