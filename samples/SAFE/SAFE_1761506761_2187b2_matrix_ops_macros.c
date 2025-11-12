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
        if (sscanf(buffer, "%d", &value) != 1) continue;
        if (value >= min && value <= max) return value;
    }
}

static void matrix_multiply(const int* a, const int* b, int* result, 
                           int a_rows, int a_cols, int b_cols) {
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

static void read_matrix(int* mat, int rows, int cols, const char* name) {
    printf("Enter values for matrix %s (%dx%d):\n", name, rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "[%d,%d]: ", i, j);
            MATRIX_IDX(mat, i, j, cols) = read_int(prompt, -1000, 1000);
        }
    }
}

int main(void) {
    int a_rows = read_int("Enter rows for matrix A (1-10): ", 1, MAX_DIM);
    int a_cols = read_int("Enter cols for matrix A (1-10): ", 1, MAX_DIM);
    int b_rows = read_int("Enter rows for matrix B (1-10): ", 1, MAX_DIM);
    int b_cols = read_int("Enter cols for matrix B (1-10): ", 1, MAX_DIM);
    
    if (a_cols != b_rows) {
        printf("Matrix dimensions incompatible for multiplication.\n");
        return 1;
    }
    
    int matrix_a[MAX_DIM * MAX_DIM];
    int matrix_b[MAX_DIM * MAX_DIM];
    int result[MAX_DIM * MAX_DIM];
    
    memset(matrix_a, 0, sizeof(matrix_a));
    memset(matrix_b, 0, sizeof(matrix_b));
    memset(result, 0, sizeof(result));
    
    read_matrix(matrix_a, a_rows, a_cols, "A");
    read_matrix(matrix_b, b_rows, b_cols, "B");
    
    matrix_multiply(matrix_a, matrix_b, result, a_rows, a_cols, b_cols);
    
    printf("\nMatrix A:\n");
    print_matrix(matrix_a, a_rows, a_cols);
    
    printf("\nMatrix B:\n");
    print_matrix(matrix_b, b_rows, b_cols);
    
    printf("\nResult (A * B):\n");
    print_matrix(result, a_rows, b_cols);
    
    return 0;
}