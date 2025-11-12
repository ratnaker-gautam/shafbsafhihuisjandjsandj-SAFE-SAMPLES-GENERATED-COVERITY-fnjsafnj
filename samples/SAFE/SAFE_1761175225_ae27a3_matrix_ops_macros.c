//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10
#define MATRIX_IDX(mat, row, col, cols) ((mat)[(row) * (cols) + (col)])

static int read_int_safe(const char* prompt, int min_val, int max_val) {
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

static void matrix_multiply(const int* a, const int* b, int* result, int rows_a, int cols_a, int cols_b) {
    for (int i = 0; i < rows_a; i++) {
        for (int j = 0; j < cols_b; j++) {
            int sum = 0;
            for (int k = 0; k < cols_a; k++) {
                sum += MATRIX_IDX(a, i, k, cols_a) * MATRIX_IDX(b, k, j, cols_b);
            }
            MATRIX_IDX(result, i, j, cols_b) = sum;
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

int main(void) {
    int rows_a, cols_a, rows_b, cols_b;
    
    printf("Matrix A dimensions:\n");
    rows_a = read_int_safe("Rows (1-10): ", 1, MAX_DIM);
    cols_a = read_int_safe("Columns (1-10): ", 1, MAX_DIM);
    
    printf("Matrix B dimensions:\n");
    rows_b = read_int_safe("Rows (1-10): ", 1, MAX_DIM);
    cols_b = read_int_safe("Columns (1-10): ", 1, MAX_DIM);
    
    if (cols_a != rows_b) {
        printf("Error: Matrix A columns must equal Matrix B rows for multiplication.\n");
        return 1;
    }
    
    int matrix_a[MAX_DIM * MAX_DIM];
    int matrix_b[MAX_DIM * MAX_DIM];
    int result[MAX_DIM * MAX_DIM];
    
    printf("Enter Matrix A elements:\n");
    for (int i = 0; i < rows_a; i++) {
        for (int j = 0; j < cols_a; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "A[%d][%d]: ", i, j);
            MATRIX_IDX(matrix_a, i, j, cols_a) = read_int_safe(prompt, -1000, 1000);
        }
    }
    
    printf("Enter Matrix B elements:\n");
    for (int i = 0; i < rows_b; i++) {
        for (int j = 0; j < cols_b; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "B[%d][%d]: ", i, j);
            MATRIX_IDX(matrix_b, i, j, cols_b) = read_int_safe(prompt, -1000, 1000);
        }
    }
    
    memset(result, 0, sizeof(result));
    matrix_multiply(matrix_a, matrix_b, result, rows_a, cols_a, cols_b);
    
    printf("Matrix A:\n");
    print_matrix(matrix_a, rows_a, cols_a);
    
    printf("Matrix B:\n");
    print_matrix(matrix_b, rows_b, cols_b);
    
    printf("Result (A * B):\n");
    print_matrix(result, rows_a, cols_b);
    
    return 0;
}