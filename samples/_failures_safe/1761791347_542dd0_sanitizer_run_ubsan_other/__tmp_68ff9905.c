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

int main(void) {
    int a_rows, a_cols, b_rows, b_cols;
    
    printf("Matrix A dimensions:\n");
    a_rows = read_int("Rows (1-10): ", 1, MAX_DIM);
    a_cols = read_int("Columns (1-10): ", 1, MAX_DIM);
    
    printf("Matrix B dimensions:\n");
    b_rows = read_int("Rows (1-10): ", 1, MAX_DIM);
    b_cols = read_int("Columns (1-10): ", 1, MAX_DIM);
    
    if (a_cols != b_rows) {
        printf("Error: Matrix A columns (%d) must equal Matrix B rows (%d) for multiplication.\n", a_cols, b_rows);
        return 1;
    }
    
    int matrix_a[MAX_DIM * MAX_DIM];
    int matrix_b[MAX_DIM * MAX_DIM];
    int result[MAX_DIM * MAX_DIM];
    
    printf("Enter Matrix A elements:\n");
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < a_cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "A[%d][%d]: ", i, j);
            MATRIX_IDX(matrix_a, i, j, a_cols) = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
    
    printf("Enter Matrix B elements:\n");
    for (int i = 0; i < b_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "B[%d][%d]: ", i, j);
            MATRIX_IDX(matrix_b, i, j, b_cols) = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
    
    memset(result, 0, sizeof(result));
    matrix_multiply(matrix_a, a_rows, a_cols, matrix_b, b_rows, b_cols, result);
    
    printf("\nMatrix A:\n");
    print_matrix(matrix_a, a_rows, a_cols);
    
    printf("\nMatrix B:\n");
    print_matrix(matrix_b, b_rows, b_cols);
    
    printf("\nResult (A * B):\n");
    print_matrix(result, a_rows, b_cols);
    
    return 0;
}