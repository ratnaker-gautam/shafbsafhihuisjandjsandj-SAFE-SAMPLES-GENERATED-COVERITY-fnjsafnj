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
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min_val;
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
    int rows1, cols1, rows2, cols2;
    
    printf("Matrix Multiplication Demo\n");
    printf("Matrix dimensions limited to %dx%d\n", MAX_DIM, MAX_DIM);
    
    rows1 = read_int("Enter rows for matrix A: ", 1, MAX_DIM);
    cols1 = read_int("Enter columns for matrix A: ", 1, MAX_DIM);
    rows2 = read_int("Enter rows for matrix B: ", 1, MAX_DIM);
    cols2 = read_int("Enter columns for matrix B: ", 1, MAX_DIM);
    
    if (cols1 != rows2) {
        printf("Error: Matrix A columns (%d) must equal Matrix B rows (%d) for multiplication.\n", cols1, rows2);
        return 1;
    }
    
    int matrix_a[MAX_DIM * MAX_DIM];
    int matrix_b[MAX_DIM * MAX_DIM];
    int result[MAX_DIM * MAX_DIM];
    
    memset(matrix_a, 0, sizeof(matrix_a));
    memset(matrix_b, 0, sizeof(matrix_b));
    memset(result, 0, sizeof(result));
    
    srand(42);
    fill_matrix(matrix_a, rows1, cols1);
    fill_matrix(matrix_b, rows2, cols2);
    
    printf("\nMatrix A (%dx%d):\n", rows1, cols1);
    print_matrix(matrix_a, rows1, cols1);
    
    printf("\nMatrix B (%dx%d):\n", rows2, cols2);
    print_matrix(matrix_b, rows2, cols2);
    
    matrix_multiply(matrix_a, rows1, cols1, matrix_b, rows2, cols2, result);
    
    printf("\nResult Matrix (%dx%d):\n", rows1, cols2);
    print_matrix(result, rows1, cols2);
    
    return 0;
}