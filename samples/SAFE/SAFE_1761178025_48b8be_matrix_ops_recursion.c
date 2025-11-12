//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 10

int read_matrix(int matrix[MAX_SIZE][MAX_SIZE], int *rows, int *cols) {
    if (scanf("%d %d", rows, cols) != 2) return 0;
    if (*rows <= 0 || *rows > MAX_SIZE) return 0;
    if (*cols <= 0 || *cols > MAX_SIZE) return 0;
    for (int i = 0; i < *rows; i++) {
        for (int j = 0; j < *cols; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) return 0;
        }
    }
    return 1;
}

void print_matrix(int matrix[MAX_SIZE][MAX_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void transpose_recursive(int src[MAX_SIZE][MAX_SIZE], int dst[MAX_SIZE][MAX_SIZE], int i, int j, int rows, int cols) {
    if (i >= rows) return;
    if (j >= cols) {
        transpose_recursive(src, dst, i + 1, 0, rows, cols);
        return;
    }
    dst[j][i] = src[i][j];
    transpose_recursive(src, dst, i, j + 1, rows, cols);
}

int multiply_recursive(int a[MAX_SIZE][MAX_SIZE], int b[MAX_SIZE][MAX_SIZE], int c[MAX_SIZE][MAX_SIZE], int i, int j, int k, int arows, int acols, int bcols) {
    if (i >= arows) return 1;
    if (j >= bcols) {
        return multiply_recursive(a, b, c, i + 1, 0, 0, arows, acols, bcols);
    }
    if (k >= acols) {
        return multiply_recursive(a, b, c, i, j + 1, 0, arows, acols, bcols);
    }
    c[i][j] += a[i][k] * b[k][j];
    return multiply_recursive(a, b, c, i, j, k + 1, arows, acols, bcols);
}

int main() {
    int a[MAX_SIZE][MAX_SIZE] = {0};
    int b[MAX_SIZE][MAX_SIZE] = {0};
    int c[MAX_SIZE][MAX_SIZE] = {0};
    int arows, acols, brows, bcols;

    printf("Enter first matrix dimensions and elements:\n");
    if (!read_matrix(a, &arows, &acols)) {
        printf("Invalid input for first matrix\n");
        return 1;
    }

    printf("Enter second matrix dimensions and elements:\n");
    if (!read_matrix(b, &brows, &bcols)) {
        printf("Invalid input for second matrix\n");
        return 1;
    }

    printf("\nFirst matrix:\n");
    print_matrix(a, arows, acols);
    printf("\nSecond matrix:\n");
    print_matrix(b, brows, bcols);

    int transposed[MAX_SIZE][MAX_SIZE] = {0};
    printf("\nTransposed first matrix:\n");
    transpose_recursive(a, transposed, 0, 0, arows, acols);
    print_matrix(transposed, acols, arows);

    if (acols == brows) {
        memset(c, 0, sizeof(c));
        if (multiply_recursive(a, b, c, 0, 0, 0, arows, acols, bcols)) {
            printf("\nMatrix multiplication result:\n");
            print_matrix(c, arows, bcols);
        } else {
            printf("\nMatrix multiplication failed\n");
        }
    } else {
        printf("\nMatrices cannot be multiplied\n");
    }

    return 0;
}