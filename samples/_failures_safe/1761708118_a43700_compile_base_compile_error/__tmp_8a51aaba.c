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
    if (!matrix) return;
    for (int i = 0; i < rows; i++) {
        if (!matrix[i]) return;
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
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

int read_int(const char *prompt, int min, int max) {
    int value;
    char buffer[100];
    while (1) {
        printf("%s", prompt);
        if (!fgets(buffer, sizeof(buffer), stdin)) continue;
        if (sscanf(buffer, "%d", &value) != 1) continue;
        if (value >= min && value <= max) break;
    }
    return value;
}

void read_matrix(int **matrix, int rows, int cols) {
    if (!matrix) return;
    for (int i = 0; i < rows; i++) {
        if (!matrix[i]) return;
        for (int j = 0; j < cols; j++) {
            char prompt[50];
            snprintf(prompt, sizeof(prompt), "Element [%d][%d]: ", i, j);
            matrix[i][j] = read_int(prompt, INT_MIN, INT_MAX);
        }
    }
}

int **add_matrices(int **a, int **b, int rows, int cols) {
    if (!a || !b) return NULL;
    int **result = allocate_matrix(rows, cols);
    if (!result) return NULL;
    for (int i = 0; i < rows; i++) {
        if (!a[i] || !b[i]) {
            free_matrix(result, rows);
            return NULL;
        }
        for (int j = 0; j < cols; j++) {
            if ((a[i][j] > 0 && b[i][j] > INT_MAX - a[i][j]) ||
                (a[i][j] < 0 && b[i][j] < INT_MIN - a[i][j])) {
                free_matrix(result, rows);
                return NULL;
            }
            result[i][j] = a[i][j] + b[i][j];
        }
    }
    return result;
}

int **subtract_matrices(int **a, int **b, int rows, int cols) {
    if (!a || !b) return NULL;
    int **result = allocate_matrix(rows, cols);
    if (!result) return NULL;
    for (int i = 0; i < rows; i++) {
        if (!a[i] || !b[i]) {
            free_matrix(result, rows);
            return NULL;
        }
        for (int j = 0; j < cols; j++) {
            if ((b[i][j] > 0 && a[i][j] < INT_MIN + b[i][j]) ||
                (b[i][j] < 0 && a[i][j] > INT_MAX + b[i][j])) {
                free_matrix(result, rows);
                return NULL;
            }
            result[i][j] = a[i][j] - b[i][j];
        }
    }
    return result;
}

int **multiply_matrices(int **a, int **b, int rows1, int cols1, int cols2) {
    if (!a || !b) return NULL;
    int **result = allocate_matrix(rows1, cols2);
    if (!result) return NULL;
    for (int i = 0; i < rows1; i++) {
        if (!a[i]) {
            free_matrix(result, rows1);
            return NULL;
        }
        for (int j = 0; j < cols2; j++) {
            result[i][j] = 0;
            for (int k = 0; k < cols1; k++) {
                if (!b[k]) {
                    free_matrix(result, rows1);
                    return NULL;
                }
                long long product = (long long)a[i][k] * b[k][j];
                if (product > INT_MAX || product < INT_MIN) {
                    free_matrix(result, rows1);
                    return NULL;
                }
                if (result[i][j] > INT_MAX - (int)product ||
                    result[i][j] < INT_MIN - (int)product) {
                    free_matrix(result, rows1);
                    return NULL;
                }
                result[i][j] += (int)product;
            }
        }
    }
    return result;
}

int **transpose_matrix(int **matrix, int