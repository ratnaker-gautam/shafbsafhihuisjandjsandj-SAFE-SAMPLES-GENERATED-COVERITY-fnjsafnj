//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

enum MatrixOp {
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_TRANSPOSE,
    OP_DETERMINANT
};

void print_matrix(int **matrix, int rows, int cols) {
    if (!matrix || rows <= 0 || cols <= 0) return;
    for (int i = 0; i < rows; i++) {
        if (!matrix[i]) return;
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int safe_add(int a, int b) {
    if ((b > 0 && a > INT_MAX - b) || (b < 0 && a < INT_MIN - b)) {
        return 0;
    }
    return a + b;
}

int safe_multiply(int a, int b) {
    if (a > 0) {
        if (b > 0) {
            if (a > INT_MAX / b) return 0;
        } else {
            if (b < INT_MIN / a) return 0;
        }
    } else {
        if (b > 0) {
            if (a < INT_MIN / b) return 0;
        } else {
            if (a != 0 && b < INT_MAX / a) return 0;
        }
    }
    return a * b;
}

int **allocate_matrix(int rows, int cols) {
    if (rows <= 0 || cols <= 0) return NULL;
    int **matrix = malloc(rows * sizeof(int *));
    if (!matrix) return NULL;
    for (int i = 0; i < rows; i++) {
        matrix[i] = malloc(cols * sizeof(int));
        if (!matrix[i]) {
            for (int j = 0; j < i; j++) free(matrix[j]);
            free(matrix);
            return NULL;
        }
    }
    return matrix;
}

void free_matrix(int **matrix, int rows) {
    if (!matrix) return;
    for (int i = 0; i < rows; i++) free(matrix[i]);
    free(matrix);
}

int **add_matrices(int **a, int **b, int rows, int cols) {
    if (!a || !b || rows <= 0 || cols <= 0) return NULL;
    int **result = allocate_matrix(rows, cols);
    if (!result) return NULL;
    for (int i = 0; i < rows; i++) {
        if (!a[i] || !b[i]) {
            free_matrix(result, rows);
            return NULL;
        }
        for (int j = 0; j < cols; j++) {
            result[i][j] = safe_add(a[i][j], b[i][j]);
        }
    }
    return result;
}

int **subtract_matrices(int **a, int **b, int rows, int cols) {
    if (!a || !b || rows <= 0 || cols <= 0) return NULL;
    int **result = allocate_matrix(rows, cols);
    if (!result) return NULL;
    for (int i = 0; i < rows; i++) {
        if (!a[i] || !b[i]) {
            free_matrix(result, rows);
            return NULL;
        }
        for (int j = 0; j < cols; j++) {
            result[i][j] = safe_add(a[i][j], -b[i][j]);
        }
    }
    return result;
}

int **multiply_matrices(int **a, int **b, int rows1, int cols1, int rows2, int cols2) {
    if (!a || !b || rows1 <= 0 || cols1 <= 0 || rows2 <= 0 || cols2 <= 0 || cols1 != rows2) return NULL;
    int **result = allocate_matrix(rows1, cols2);
    if (!result) return NULL;
    for (int i = 0; i < rows1; i++) {
        if (!a[i]) {
            free_matrix(result, rows1);
            return NULL;
        }
        for (int j = 0; j < cols2; j++) {
            int sum = 0;
            for (int k = 0; k < cols1; k++) {
                if (!b[k]) {
                    free_matrix(result, rows1);
                    return NULL;
                }
                int product = safe_multiply(a[i][k], b[k][j]);
                if ((product > 0 && sum > INT_MAX - product) || (product < 0 && sum < INT_MIN - product)) {
                    free_matrix(result, rows1);
                    return NULL;
                }
                sum += product;
            }
            result[i][j] = sum;
        }
    }
    return result;
}

int **transpose_matrix(int **matrix, int rows, int cols) {
    if (!matrix || rows <= 0 || cols <= 0) return NULL;
    int **result = allocate_matrix(cols, rows);
    if (!result) return NULL;
    for (int i = 0; i < rows; i++) {
        if (!matrix[i]) {
            free_matrix(result, cols);
            return NULL;
        }
        for (int j = 0; j < cols; j++) {
            result[j][i] = matrix[i][j];
        }
    }
    return result;
}

int determinant_matrix(int **matrix, int n) {