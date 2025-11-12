//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: matrix_ops
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
        if (sscanf(buffer, "%d", &value) != 1) continue;
        if (value >= min && value <= max) return value;
    }
}

static void matrix_multiply(const int* a, int a_rows, int a_cols,
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

static void matrix_transpose(const int* src, int rows, int cols, int* dest) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            MATRIX_IDX(dest, j, i, rows) = MATRIX_IDX(src, i, j, cols);
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

static void fill_matrix(int* mat, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            MATRIX_IDX(mat, i, j, cols) = rand() % 100;
        }
    }
}

int main(void) {
    srand(42);
    
    int a_rows = read_int("Enter rows for matrix A (1-10): ", 1, MAX_DIM);
    int a_cols = read_int("Enter cols for matrix A (1-10): ", 1, MAX_DIM);
    int b_rows = read_int("Enter rows for matrix B (1-10): ", 1, MAX_DIM);
    int b_cols = read_int("Enter cols for matrix B (1-10): ", 1, MAX_DIM);
    
    if (a_cols != b_rows) {
        printf("Matrix multiplication requires A cols == B rows.\n");
        return 1;
    }
    
    int matrix_a[MAX_DIM * MAX_DIM];
    int matrix_b[MAX_DIM * MAX_DIM];
    int matrix_result[MAX_DIM * MAX_DIM];
    int matrix_transposed[MAX_DIM * MAX_DIM];
    
    fill_matrix(matrix_a, a_rows, a_cols);
    fill_matrix(matrix_b, b_rows, b_cols);
    
    printf("\nMatrix A (%dx%d):\n", a_rows, a_cols);
    print_matrix(matrix_a, a_rows, a_cols);
    
    printf("\nMatrix B (%dx%d):\n", b_rows, b_cols);
    print_matrix(matrix_b, b_rows, b_cols);
    
    matrix_multiply(matrix_a, a_rows, a_cols, matrix_b, b_rows, b_cols, matrix_result);
    printf("\nMatrix A * B (%dx%d):\n", a_rows, b_cols);
    print_matrix(matrix_result, a_rows, b_cols);
    
    matrix_transpose(matrix_result, a_rows, b_cols, matrix_transposed);
    printf("\nTranspose of A * B (%dx%d):\n", b_cols, a_rows);
    print_matrix(matrix_transposed, b_cols, a_rows);
    
    return 0;
}