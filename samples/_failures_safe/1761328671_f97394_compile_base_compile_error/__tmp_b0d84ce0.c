//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10
#define MATRIX_ELEM(mat, i, j, cols) ((mat)[(i) * (cols) + (j)])

int read_int(const char* prompt, int min, int max) {
    char buffer[32];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min;
        }
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

void print_matrix(int* mat, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", MATRIX_ELEM(mat, i, j, cols));
        }
        printf("\n");
    }
}

void multiply_matrices(int* a, int a_rows, int a_cols, int* b, int b_rows, int b_cols, int* result) {
    for (int i = 0; i < a_rows; i++) {
        for (int j = 0; j < b_cols; j++) {
            int sum = 0;
            for (int k = 0; k < a_cols; k++) {
                if (MATRIX_ELEM(a, i, k, a_cols) > 0 && MATRIX_ELEM(b, k, j, b_cols) > 0) {
                    if (MATRIX_ELEM(a, i, k, a_cols) > INT_MAX / MATRIX_ELEM(b, k, j, b_cols)) {
                        sum = INT_MAX;
                        break;
                    }
                } else if (MATRIX_ELEM(a, i, k, a_cols) < 0 && MATRIX_ELEM(b, k, j, b_cols) < 0) {
                    if (MATRIX_ELEM(a, i, k, a_cols) < INT_MIN / MATRIX_ELEM(b, k, j, b_cols)) {
                        sum = INT_MIN;
                        break;
                    }
                }
                sum += MATRIX_ELEM(a, i, k, a_cols) * MATRIX_ELEM(b, k, j, b_cols);
                if ((sum > 0 && MATRIX_ELEM(a, i, k, a_cols) > 0 && MATRIX_ELEM(b, k, j, b_cols) > 0 && sum < 0) ||
                    (sum < 0 && MATRIX_ELEM(a, i, k, a_cols) < 0 && MATRIX_ELEM(b, k, j, b_cols) < 0 && sum > 0)) {
                    sum = (MATRIX_ELEM(a, i, k, a_cols) > 0) ? INT_MAX : INT_MIN;
                    break;
                }
            }
            MATRIX_ELEM(result, i, j, b_cols) = sum;
        }
    }
}

int main() {
    int rows1, cols1, rows2, cols2;
    
    printf("Matrix Multiplication Program\n");
    printf("First matrix dimensions:\n");
    rows1 = read_int("Rows (1-10): ", 1, MAX_DIM);
    cols1 = read_int("Columns (1-10): ", 1, MAX_DIM);
    
    printf("Second matrix dimensions:\n");
    rows2 = read_int("Rows (1-10): ", 1, MAX_DIM);
    cols2 = read_int("Columns (1-10): ", 1, MAX_DIM);
    
    if (cols1 != rows2) {
        printf("Error: Columns of first matrix must equal rows of second matrix.\n");
        return 1;
    }
    
    int matrix1[MAX_DIM * MAX_DIM];
    int matrix2[MAX_DIM * MAX_DIM];
    int result[MAX_DIM * MAX_DIM];
    
    printf("Enter elements for first matrix (%d x %d):\n", rows1, cols1);
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols1; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            MATRIX_ELEM(matrix1, i, j, cols1) = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
    
    printf("Enter elements for second matrix (%d x %d):\n", rows2, cols2);
    for (int i = 0; i < rows2; i++) {
        for (int j = 0; j < cols2; j++) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            MATRIX_ELEM(matrix2, i, j, cols