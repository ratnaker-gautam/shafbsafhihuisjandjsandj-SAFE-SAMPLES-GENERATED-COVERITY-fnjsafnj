//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#define MAX_SIZE 10

void print_matrix(int matrix[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d", matrix[i][j]);
        }
        printf("\n");
    }
}

void transpose_matrix(int src[MAX_SIZE][MAX_SIZE], int dest[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    int i = 0;
    while (i < rows) {
        int j = 0;
        do {
            dest[j][i] = src[i][j];
            j++;
        } while (j < cols);
        i++;
    }
}

void multiply_matrices(int a[MAX_SIZE][MAX_SIZE], int b[MAX_SIZE][MAX_SIZE], int result[MAX_SIZE][MAX_SIZE], int rows_a, int cols_a, int cols_b) {
    for (int i = 0; i < rows_a; i++) {
        for (int j = 0; j < cols_b; j++) {
            result[i][j] = 0;
            int k = 0;
            while (k < cols_a) {
                if (a[i][k] > 0 && b[k][j] > INT_MAX / a[i][k]) {
                    printf("Integer overflow detected in multiplication\n");
                    return;
                }
                if (a[i][k] < 0 && b[k][j] < INT_MAX / a[i][k]) {
                    printf("Integer overflow detected in multiplication\n");
                    return;
                }
                result[i][j] += a[i][k] * b[k][j];
                k++;
            }
        }
    }
}

int main() {
    int matrix_a[MAX_SIZE][MAX_SIZE];
    int matrix_b[MAX_SIZE][MAX_SIZE];
    int result[MAX_SIZE][MAX_SIZE];
    int rows_a, cols_a, rows_b, cols_b;

    printf("Enter dimensions for matrix A (rows columns, max %d): ", MAX_SIZE);
    if (scanf("%d %d", &rows_a, &cols_a) != 2 || rows_a <= 0 || cols_a <= 0 || rows_a > MAX_SIZE || cols_a > MAX_SIZE) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }

    printf("Enter dimensions for matrix B (rows columns, max %d): ", MAX_SIZE);
    if (scanf("%d %d", &rows_b, &cols_b) != 2 || rows_b <= 0 || cols_b <= 0 || rows_b > MAX_SIZE || cols_b > MAX_SIZE) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }

    printf("Enter elements for matrix A (%d x %d):\n", rows_a, cols_a);
    for (int i = 0; i < rows_a; i++) {
        for (int j = 0; j < cols_a; j++) {
            if (scanf("%d", &matrix_a[i][j]) != 1) {
                printf("Invalid input\n");
                return 1;
            }
        }
    }

    printf("Enter elements for matrix B (%d x %d):\n", rows_b, cols_b);
    for (int i = 0; i < rows_b; i++) {
        int j = 0;
        while (j < cols_b) {
            if (scanf("%d", &matrix_b[i][j]) != 1) {
                printf("Invalid input\n");
                return 1;
            }
            j++;
        }
    }

    printf("\nMatrix A:\n");
    print_matrix(matrix_a, rows_a, cols_a);

    printf("\nMatrix B:\n");
    print_matrix(matrix_b, rows_b, cols_b);

    printf("\nTranspose of A:\n");
    int transposed[MAX_SIZE][MAX_SIZE];
    transpose_matrix(matrix_a, transposed, rows_a, cols_a);
    print_matrix(transposed, cols_a, rows_a);

    if (cols_a == rows_b) {
        printf("\nMatrix multiplication A * B:\n");
        multiply_matrices(matrix_a, matrix_b, result, rows_a, cols_a, cols_b);
        print_matrix(result, rows_a, cols_b);
    } else {
        printf("\nMatrix multiplication not possible: columns of A must equal rows of B\n");
    }

    return 0;
}