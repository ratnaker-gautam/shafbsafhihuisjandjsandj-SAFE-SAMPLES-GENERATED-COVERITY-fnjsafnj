//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10
#define MATRIX_ELEM(mat, i, j, cols) ((mat)[(i) * (cols) + (j)])

int read_int_safe(int *value) {
    char buffer[32];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return 0;
    char *endptr;
    long val = strtol(buffer, &endptr, 10);
    if (endptr == buffer || *endptr != '\n') return 0;
    if (val < INT_MIN || val > INT_MAX) return 0;
    *value = (int)val;
    return 1;
}

int read_matrix(int *matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("Enter element [%d][%d]: ", i, j);
            int value;
            if (!read_int_safe(&value)) return 0;
            MATRIX_ELEM(matrix, i, j, cols) = value;
        }
    }
    return 1;
}

void print_matrix(int *matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", MATRIX_ELEM(matrix, i, j, cols));
        }
        printf("\n");
    }
}

int multiply_matrices(int *a, int a_rows, int a_cols, int *b, int b_rows, int b_cols, int *result) {
    if (a_cols != b_rows) return 0;
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            int sum = 0;
            for (int k = 0; k < a_cols; k++) {
                sum += MATRIX_ELEM(a, i, k, a_cols) * MATRIX_ELEM(b, k, j, b_cols);
            }
            MATRIX_ELEM(result, i, j, b_cols) = sum;
        }
    }
    return 1;
}

int main() {
    int a_rows, a_cols, b_rows, b_cols;
    
    printf("Enter dimensions of matrix A (rows cols): ");
    if (!read_int_safe(&a_rows) || !read_int_safe(&a_cols)) {
        printf("Invalid input for matrix A dimensions\n");
        return 1;
    }
    if (a_rows <= 0 || a_cols <= 0 || a_rows > MAX_DIM || a_cols > MAX_DIM) {
        printf("Matrix A dimensions out of range (1-%d)\n", MAX_DIM);
        return 1;
    }
    
    printf("Enter dimensions of matrix B (rows cols): ");
    if (!read_int_safe(&b_rows) || !read_int_safe(&b_cols)) {
        printf("Invalid input for matrix B dimensions\n");
        return 1;
    }
    if (b_rows <= 0 || b_cols <= 0 || b_rows > MAX_DIM || b_cols > MAX_DIM) {
        printf("Matrix B dimensions out of range (1-%d)\n", MAX_DIM);
        return 1;
    }
    
    if (a_cols != b_rows) {
        printf("Matrix multiplication not possible: A cols (%d) != B rows (%d)\n", a_cols, b_rows);
        return 1;
    }
    
    int matrix_a[MAX_DIM * MAX_DIM];
    int matrix_b[MAX_DIM * MAX_DIM];
    int result[MAX_DIM * MAX_DIM];
    
    printf("Enter elements for matrix A:\n");
    if (!read_matrix(matrix_a, a_rows, a_cols)) {
        printf("Invalid input for matrix A elements\n");
        return 1;
    }
    
    printf("Enter elements for matrix B:\n");
    if (!read_matrix(matrix_b, b_rows, b_cols)) {
        printf("Invalid input for matrix B elements\n");
        return 1;
    }
    
    memset(result, 0, sizeof(result));
    if (!multiply_matrices(matrix_a, a_rows, a_cols, matrix_b, b_rows, b_cols, result)) {
        printf("Matrix multiplication failed\n");
        return 1;
    }
    
    printf("\nMatrix A:\n");
    print_matrix(matrix_a, a_rows, a_cols);
    
    printf("\nMatrix B:\n");
    print_matrix(matrix_b, b_rows, b_cols);
    
    printf("\nResult (A * B):\n");
    print_matrix(result, a_rows, b_cols);
    
    return 0;
}