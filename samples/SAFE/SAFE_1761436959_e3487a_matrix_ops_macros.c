//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_ROWS 10
#define MAX_COLS 10
#define MIN_DIM 1

#define MATRIX_ELEM(m, i, j, cols) ((m)[(i) * (cols) + (j)])

#define SWAP(a, b) do { \
    int temp = (a); \
    (a) = (b); \
    (b) = temp; \
} while(0)

int read_int(const char* prompt, int min_val, int max_val) {
    char buffer[32];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return -1;
        }
        if (sscanf(buffer, "%d", &value) != 1) {
            printf("Invalid input. Please enter an integer.\n");
            continue;
        }
        if (value < min_val || value > max_val) {
            printf("Value must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        return value;
    }
}

void read_matrix(int* matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[64];
            snprintf(prompt, sizeof(prompt), "Enter element [%d][%d]: ", i, j);
            MATRIX_ELEM(matrix, i, j, cols) = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

void print_matrix(const int* matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", MATRIX_ELEM(matrix, i, j, cols));
        }
        printf("\n");
    }
}

void transpose_matrix(const int* src, int* dest, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            MATRIX_ELEM(dest, j, i, rows) = MATRIX_ELEM(src, i, j, cols);
        }
    }
}

void multiply_matrices(const int* a, const int* b, int* result, int a_rows, int a_cols, int b_cols) {
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

int main() {
    int rows1, cols1, rows2, cols2;
    
    printf("Matrix Operations Program\n");
    
    rows1 = read_int("Enter rows for matrix 1: ", MIN_DIM, MAX_ROWS);
    if (rows1 == -1) return 1;
    cols1 = read_int("Enter columns for matrix 1: ", MIN_DIM, MAX_COLS);
    if (cols1 == -1) return 1;
    
    int matrix1[MAX_ROWS * MAX_COLS];
    printf("Enter elements for matrix 1:\n");
    read_matrix(matrix1, rows1, cols1);
    
    printf("\nMatrix 1:\n");
    print_matrix(matrix1, rows1, cols1);
    
    int transposed[MAX_ROWS * MAX_COLS];
    transpose_matrix(matrix1, transposed, rows1, cols1);
    printf("\nTranspose of Matrix 1:\n");
    print_matrix(transposed, cols1, rows1);
    
    rows2 = read_int("\nEnter rows for matrix 2: ", MIN_DIM, MAX_ROWS);
    if (rows2 == -1) return 1;
    cols2 = read_int("Enter columns for matrix 2: ", MIN_DIM, MAX_COLS);
    if (cols2 == -1) return 1;
    
    if (cols1 != rows2) {
        printf("Matrix multiplication not possible: columns of matrix 1 must equal rows of matrix 2.\n");
        return 1;
    }
    
    int matrix2[MAX_ROWS * MAX_COLS];
    printf("Enter elements for matrix 2:\n");
    read_matrix(matrix2, rows2, cols2);
    
    printf("\nMatrix 2:\n");
    print_matrix(matrix2, rows2, cols2);
    
    int product[MAX_ROWS * MAX_COLS];
    multiply_matrices(matrix1, matrix2, product, rows1, cols1, cols2);
    printf("\nMatrix 1 * Matrix 2:\n");
    print_matrix(product, rows1, cols2);
    
    return 0;
}