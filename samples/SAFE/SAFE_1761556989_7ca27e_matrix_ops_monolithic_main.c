//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_DIM 10

int read_int(const char* prompt, int min_val, int max_val) {
    char buffer[32];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            return min_val;
        }
        if (sscanf(buffer, "%d", &value) == 1 && value >= min_val && value <= max_val) {
            return value;
        }
        printf("Invalid input. Enter integer between %d and %d.\n", min_val, max_val);
    }
}

void fill_matrix(int rows, int cols, int matrix[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = read_int("Enter matrix element: ", -1000, 1000);
        }
    }
}

void print_matrix(int rows, int cols, int matrix[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6d", matrix[i][j]);
        }
        printf("\n");
    }
}

void multiply_matrices(int m, int n, int p, int a[MAX_DIM][MAX_DIM], int b[MAX_DIM][MAX_DIM], int result[MAX_DIM][MAX_DIM]) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            result[i][j] = 0;
            for (int k = 0; k < n; k++) {
                if (a[i][k] > 0 && b[k][j] > 0 && a[i][k] > INT_MAX / b[k][j]) {
                    result[i][j] = 0;
                    break;
                } else if (a[i][k] < 0 && b[k][j] < 0 && a[i][k] < INT_MAX / b[k][j]) {
                    result[i][j] = 0;
                    break;
                } else if (a[i][k] > 0 && b[k][j] < 0 && a[i][k] > INT_MIN / b[k][j]) {
                    result[i][j] = 0;
                    break;
                } else if (a[i][k] < 0 && b[k][j] > 0 && a[i][k] < INT_MIN / b[k][j]) {
                    result[i][j] = 0;
                    break;
                }
                long long product = (long long)a[i][k] * (long long)b[k][j];
                if (product > INT_MAX || product < INT_MIN) {
                    result[i][j] = 0;
                    break;
                }
                result[i][j] += (int)product;
            }
        }
    }
}

int main(void) {
    int rows1, cols1, rows2, cols2;
    int matrix1[MAX_DIM][MAX_DIM];
    int matrix2[MAX_DIM][MAX_DIM];
    int result[MAX_DIM][MAX_DIM];

    printf("Matrix Multiplication Program\n");
    printf("Matrix dimensions must be between 1 and %d\n", MAX_DIM);

    rows1 = read_int("Enter rows for first matrix: ", 1, MAX_DIM);
    cols1 = read_int("Enter columns for first matrix: ", 1, MAX_DIM);
    rows2 = read_int("Enter rows for second matrix: ", 1, MAX_DIM);
    cols2 = read_int("Enter columns for second matrix: ", 1, MAX_DIM);

    if (cols1 != rows2) {
        printf("Error: Columns of first matrix must equal rows of second matrix.\n");
        return 1;
    }

    printf("Enter elements for first matrix:\n");
    fill_matrix(rows1, cols1, matrix1);
    printf("Enter elements for second matrix:\n");
    fill_matrix(rows2, cols2, matrix2);

    printf("\nFirst matrix:\n");
    print_matrix(rows1, cols1, matrix1);
    printf("\nSecond matrix:\n");
    print_matrix(rows2, cols2, matrix2);

    multiply_matrices(rows1, cols1, cols2, matrix1, matrix2, result);

    printf("\nResult matrix:\n");
    print_matrix(rows1, cols2, result);

    return 0;
}