//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10
#define MATRIX_IDX(r, c, cols) ((r) * (cols) + (c))
#define SWAP(a, b) do { int temp = (a); (a) = (b); (b) = temp; } while (0)

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

void print_matrix(int* mat, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d", mat[MATRIX_IDX(i, j, cols)]);
        }
        printf("\n");
    }
}

void fill_matrix(int* mat, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            mat[MATRIX_IDX(i, j, cols)] = (i * cols + j) % 20;
        }
    }
}

void transpose_matrix(int* src, int* dst, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            dst[MATRIX_IDX(j, i, rows)] = src[MATRIX_IDX(i, j, cols)];
        }
    }
}

void multiply_matrices(int* a, int* b, int* result, int a_rows, int a_cols, int b_cols) {
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            int sum = 0;
            for (int k = 0; k < a_cols; k++) {
                sum += a[MATRIX_IDX(i, k, a_cols)] * b[MATRIX_IDX(k, j, b_cols)];
            }
            result[MATRIX_IDX(i, j, b_cols)] = sum;
        }
    }
}

int main() {
    int rows1, cols1, rows2, cols2;
    
    printf("Matrix Operations Program\n");
    printf("Matrix dimensions must be between 1 and %d\n", MAX_DIM);
    
    rows1 = read_int("Enter rows for matrix 1: ", 1, MAX_DIM);
    cols1 = read_int("Enter columns for matrix 1: ", 1, MAX_DIM);
    
    int matrix1[MAX_DIM * MAX_DIM];
    fill_matrix(matrix1, rows1, cols1);
    printf("Matrix 1:\n");
    print_matrix(matrix1, rows1, cols1);
    
    int transposed[MAX_DIM * MAX_DIM];
    transpose_matrix(matrix1, transposed, rows1, cols1);
    printf("Transpose of Matrix 1:\n");
    print_matrix(transposed, cols1, rows1);
    
    rows2 = read_int("Enter rows for matrix 2: ", 1, MAX_DIM);
    cols2 = read_int("Enter columns for matrix 2: ", 1, MAX_DIM);
    
    if (cols1 != rows2) {
        printf("Cannot multiply: columns of matrix 1 (%d) must equal rows of matrix 2 (%d)\n", cols1, rows2);
        return 1;
    }
    
    int matrix2[MAX_DIM * MAX_DIM];
    fill_matrix(matrix2, rows2, cols2);
    printf("Matrix 2:\n");
    print_matrix(matrix2, rows2, cols2);
    
    int product[MAX_DIM * MAX_DIM];
    multiply_matrices(matrix1, matrix2, product, rows1, cols1, cols2);
    printf("Matrix 1 * Matrix 2:\n");
    print_matrix(product, rows1, cols2);
    
    return 0;
}