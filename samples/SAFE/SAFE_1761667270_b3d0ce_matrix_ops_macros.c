//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10
#define MATRIX_IDX(mat, row, col, cols) ((mat)[(row) * (cols) + (col)])

static int read_int(const char* prompt, int min_val, int max_val) {
    char buffer[32];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) return min_val;
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
    int a_rows, a_cols, b_rows, b_cols;
    
    printf("Matrix Multiplication Calculator\n");
    printf("Matrix dimensions must be between 1 and %d\n", MAX_DIM);
    
    a_rows = read_int("Enter rows for matrix A: ", 1, MAX_DIM);
    a_cols = read_int("Enter columns for matrix A: ", 1, MAX_DIM);
    b_rows = read_int("Enter rows for matrix B: ", 1, MAX_DIM);
    b_cols = read_int("Enter columns for matrix B: ", 1, MAX_DIM);
    
    if (a_cols != b_rows) {
        printf("Error: Matrix A columns (%d) must equal Matrix B rows (%d)\n", a_cols, b_rows);
        return 1;
    }
    
    int matrix_a[MAX_DIM * MAX_DIM];
    int matrix_b[MAX_DIM * MAX_DIM];
    int result[MAX_DIM * MAX_DIM];
    
    memset(matrix_a, 0, sizeof(matrix_a));
    memset(matrix_b, 0, sizeof(matrix_b));
    memset(result, 0, sizeof(result));
    
    printf("\nFilling matrix A with random values...\n");
    fill_matrix(matrix_a, a_rows, a_cols);
    printf("Matrix A:\n");
    print_matrix(matrix_a, a_rows, a_cols);
    
    printf("\nFilling matrix B with random values...\n");
    fill_matrix(matrix_b, b_rows, b_cols);
    printf("Matrix B:\n");
    print_matrix(matrix_b, b_rows, b_cols);
    
    printf("\nCalculating A × B...\n");
    matrix_multiply(matrix_a, a_rows, a_cols, matrix_b, b_rows, b_cols, result);
    
    printf("Result matrix (%d × %d):\n", a_rows, b_cols);
    print_matrix(result, a_rows, b_cols);
    
    return 0;
}