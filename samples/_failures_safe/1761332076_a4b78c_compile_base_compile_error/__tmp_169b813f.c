//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

enum operation {
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_TRANSPOSE,
    OP_DETERMINANT
};

void print_matrix(int **matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int **allocate_matrix(int rows, int cols) {
    if (rows <= 0 || cols <= 0 || rows > 100 || cols > 100) {
        return NULL;
    }
    int **matrix = malloc((size_t)rows * sizeof(int *));
    if (matrix == NULL) {
        return NULL;
    }
    for (int i = 0; i < rows; i++) {
        matrix[i] = malloc((size_t)cols * sizeof(int));
        if (matrix[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(matrix[j]);
            }
            free(matrix);
            return NULL;
        }
    }
    return matrix;
}

void free_matrix(int **matrix, int rows) {
    if (matrix == NULL) {
        return;
    }
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

int **add_matrices(int **a, int **b, int rows, int cols) {
    int **result = allocate_matrix(rows, cols);
    if (result == NULL) {
        return NULL;
    }
    for (int i = 0; i < rows; i++) {
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
    int **result = allocate_matrix(rows, cols);
    if (result == NULL) {
        return NULL;
    }
    for (int i = 0; i < rows; i++) {
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

int **multiply_matrices(int **a, int **b, int rows_a, int cols_a, int cols_b) {
    int **result = allocate_matrix(rows_a, cols_b);
    if (result == NULL) {
        return NULL;
    }
    for (int i = 0; i < rows_a; i++) {
        for (int j = 0; j < cols_b; j++) {
            result[i][j] = 0;
            for (int k = 0; k < cols_a; k++) {
                long long product = (long long)a[i][k] * b[k][j];
                if (product > INT_MAX || product < INT_MIN) {
                    free_matrix(result, rows_a);
                    return NULL;
                }
                long long sum = (long long)result[i][j] + product;
                if (sum > INT_MAX || sum < INT_MIN) {
                    free_matrix(result, rows_a);
                    return NULL;
                }
                result[i][j] = (int)sum;
            }
        }
    }
    return result;
}

int **transpose_matrix(int **matrix, int rows, int cols) {
    int **result = allocate_matrix(cols, rows);
    if (result == NULL) {
        return NULL;
    }
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            result[i][j] = matrix[j][i];
        }
    }
    return result;
}

int determinant_matrix(int **matrix, int n) {
    if (n == 1) {
        return matrix[0][0];
    }
    if (n == 2) {
        long long det = (long long)matrix[0][0] * matrix[1][1] - (long long)matrix[0][1] * matrix[1][0];
        if (det > INT_MAX || det < INT_MIN) {
            return 0;
        }
        return (int)det;
    }
    int det = 0;
    int sign = 1;
    for (int i = 0; i < n; i++) {
        int **submatrix = allocate_matrix(n - 1, n - 1);
        if (submatrix == NULL) {
            return 0;
        }
        for (int j = 1; j < n; j++) {
            int col_idx = 0;
            for