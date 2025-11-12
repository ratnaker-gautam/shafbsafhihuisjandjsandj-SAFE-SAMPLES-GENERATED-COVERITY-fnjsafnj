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
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) return min;
        if (sscanf(buffer, "%d", &value) != 1) continue;
        if (value >= min && value <= max) return value;
    }
}

void fill_matrix(int* matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            matrix[MATRIX_IDX(i, j, cols)] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(const int* matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", matrix[MATRIX_IDX(i, j, cols)]);
        }
        printf("\n");
    }
}

void transpose_matrix(const int* src, int* dest, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            dest[MATRIX_IDX(j, i, rows)] = src[MATRIX_IDX(i, j, cols)];
        }
    }
}

int multiply_matrices(const int* a, int arows, int acols, const int* b, int brows, int bcols, int* result) {
    if (acols != brows) return 0;
    for (int i = 0; i < arows; i++) {
        for (int j = 0; j < bcols; j++) {
            int sum = 0;
            for (int k = 0; k < acols; k++) {
                sum += a[MATRIX_IDX(i, k, acols)] * b[MATRIX_IDX(k, j, bcols)];
            }
            result[MATRIX_IDX(i, j, bcols)] = sum;
        }
    }
    return 1;
}

int main() {
    int rows1, cols1, rows2, cols2;
    
    printf("Matrix Operations Program\n");
    
    rows1 = read_int("Enter rows for matrix 1 (1-10): ", 1, MAX_DIM);
    cols1 = read_int("Enter columns for matrix 1 (1-10): ", 1, MAX_DIM);
    
    int matrix1[MAX_DIM * MAX_DIM];
    printf("Enter values for matrix 1:\n");
    fill_matrix(matrix1, rows1, cols1);
    
    printf("\nMatrix 1:\n");
    print_matrix(matrix1, rows1, cols1);
    
    int transposed[MAX_DIM * MAX_DIM];
    transpose_matrix(matrix1, transposed, rows1, cols1);
    printf("\nTranspose of Matrix 1:\n");
    print_matrix(transposed, cols1, rows1);
    
    rows2 = read_int("\nEnter rows for matrix 2 (1-10): ", 1, MAX_DIM);
    cols2 = read_int("Enter columns for matrix 2 (1-10): ", 1, MAX_DIM);
    
    int matrix2[MAX_DIM * MAX_DIM];
    printf("Enter values for matrix 2:\n");
    fill_matrix(matrix2, rows2, cols2);
    
    printf("\nMatrix 2:\n");
    print_matrix(matrix2, rows2, cols2);
    
    if (cols1 == rows2) {
        int product[MAX_DIM * MAX_DIM];
        if (multiply_matrices(matrix1, rows1, cols1, matrix2, rows2, cols2, product)) {
            printf("\nMatrix 1 * Matrix 2:\n");
            print_matrix(product, rows1, cols2);
        }
    } else {
        printf("\nMatrix dimensions incompatible for multiplication.\n");
    }
    
    return 0;
}