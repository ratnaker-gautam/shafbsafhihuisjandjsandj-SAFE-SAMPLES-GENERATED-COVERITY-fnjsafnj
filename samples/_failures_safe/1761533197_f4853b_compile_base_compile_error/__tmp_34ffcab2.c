//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10
#define MATRIX_IDX(mat, i, j, cols) ((mat)[(i) * (cols) + (j)])

static int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) return min;
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

static void fill_matrix(int* mat, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            MATRIX_IDX(mat, i, j, cols) = read_int("Enter element: ", INT_MIN, INT_MAX);
        }
    }
}

static void print_matrix(const int* mat, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", MATRIX_IDX(mat, i, j, cols));
        }
        printf("\n");
    }
}

static void multiply_matrices(const int* a, int a_rows, int a_cols,
                             const int* b, int b_rows, int b_cols,
                             int* result) {
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

static void transpose_matrix(const int* mat, int rows, int cols, int* result) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            MATRIX_IDX(result, j, i, rows) = MATRIX_IDX(mat, i, j, cols);
        }
    }
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    
    printf("Matrix Operations Program\n");
    
    rows1 = read_int("Enter rows for matrix 1 (1-10): ", 1, MAX_DIM);
    cols1 = read_int("Enter columns for matrix 1 (1-10): ", 1, MAX_DIM);
    
    int matrix1[MAX_DIM * MAX_DIM];
    printf("Fill matrix 1:\n");
    fill_matrix(matrix1, rows1, cols1);
    
    printf("Matrix 1:\n");
    print_matrix(matrix1, rows1, cols1);
    
    rows2 = read_int("Enter rows for matrix 2 (1-10): ", 1, MAX_DIM);
    cols2 = read_int("Enter columns for matrix 2 (1-10): ", 1, MAX_DIM);
    
    int matrix2[MAX_DIM * MAX_DIM];
    printf("Fill matrix 2:\n");
    fill_matrix(matrix2, rows2, cols2);
    
    printf("Matrix 2:\n");
    print_matrix(matrix2, rows2, cols2);
    
    printf("Transpose of matrix 1:\n");
    int transpose1[MAX_DIM * MAX_DIM];
    transpose_matrix(matrix1, rows1, cols1, transpose1);
    print_matrix(transpose1, cols1, rows1);
    
    printf("Transpose of matrix 2:\n");
    int transpose2[MAX_DIM * MAX_DIM];
    transpose_matrix(matrix2, rows2, cols2, transpose2);
    print_matrix(transpose2, cols2, rows2);
    
    if (cols1 == rows2) {
        printf("Matrix multiplication (matrix1 * matrix2):\n");
        int product[MAX_DIM * MAX_DIM];
        multiply_matrices(matrix1, rows1, cols1, matrix2, rows2, cols2, product);
        print_matrix(product, rows1, cols2);
    } else {
        printf("Matrix multiplication not possible (columns of matrix1 != rows of matrix2).\n");
    }
    
    if (rows1 == rows2 && cols1 == cols2) {
        printf("Matrix addition:\n");
        int sum[MAX_DIM * MAX_DIM];
        for (int i = 0; i < rows1; i++) {
            for (int j = 0; j < cols1; j++) {
                MATRIX_IDX(sum, i, j, cols1) = MATRIX_IDX(matrix1, i, j, cols1) + MATRIX_IDX(matrix2, i, j, cols1);
            }
        }
        print_matrix(sum, rows1, cols1);
    } else {
        printf("Matrix addition not possible (