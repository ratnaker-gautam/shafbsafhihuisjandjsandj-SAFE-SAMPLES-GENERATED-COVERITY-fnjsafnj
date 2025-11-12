//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 10

int read_matrix(int matrix[MAX_SIZE][MAX_SIZE], int *rows, int *cols) {
    if (scanf("%d %d", rows, cols) != 2) return 0;
    if (*rows <= 0 || *cols <= 0 || *rows > MAX_SIZE || *cols > MAX_SIZE) return 0;
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

void transpose_recursive(int src[MAX_SIZE][MAX_SIZE], int dst[MAX_SIZE][MAX_SIZE], int r, int c, int rows, int cols) {
    if (r >= rows) return;
    if (c >= cols) {
        transpose_recursive(src, dst, r + 1, 0, rows, cols);
        return;
    }
    dst[c][r] = src[r][c];
    transpose_recursive(src, dst, r, c + 1, rows, cols);
}

int multiply_recursive(int a[MAX_SIZE][MAX_SIZE], int b[MAX_SIZE][MAX_SIZE], int res[MAX_SIZE][MAX_SIZE], int r, int c, int k, int arows, int acols, int bcols) {
    if (r >= arows) return 1;
    if (c >= bcols) {
        return multiply_recursive(a, b, res, r + 1, 0, 0, arows, acols, bcols);
    }
    if (k >= acols) {
        return multiply_recursive(a, b, res, r, c + 1, 0, arows, acols, bcols);
    }
    res[r][c] += a[r][k] * b[k][c];
    return multiply_recursive(a, b, res, r, c, k + 1, arows, acols, bcols);
}

int main() {
    int a[MAX_SIZE][MAX_SIZE] = {0};
    int b[MAX_SIZE][MAX_SIZE] = {0};
    int res[MAX_SIZE][MAX_SIZE] = {0};
    int trans[MAX_SIZE][MAX_SIZE] = {0};
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

    printf("\nTranspose of first matrix:\n");
    memset(trans, 0, sizeof(trans));
    transpose_recursive(a, trans, 0, 0, arows, acols);
    print_matrix(trans, acols, arows);

    if (acols == brows) {
        printf("\nMatrix multiplication result:\n");
        memset(res, 0, sizeof(res));
        if (multiply_recursive(a, b, res, 0, 0, 0, arows, acols, bcols)) {
            print_matrix(res, arows, bcols);
        }
    } else {
        printf("\nMatrices cannot be multiplied\n");
    }

    return 0;
}