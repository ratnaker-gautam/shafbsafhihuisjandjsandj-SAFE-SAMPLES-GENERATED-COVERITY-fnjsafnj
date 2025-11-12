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
    int **matrix = malloc(rows * sizeof(int *));
    if (matrix == NULL) {
        return NULL;
    }
    for (int i = 0; i < rows; i++) {
        matrix[i] = malloc(cols * sizeof(int));
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

int **read_matrix(int *rows, int *cols) {
    printf("Enter number of rows and columns: ");
    if (scanf("%d %d", rows, cols) != 2) {
        return NULL;
    }
    if (*rows <= 0 || *cols <= 0 || *rows > 100 || *cols > 100) {
        return NULL;
    }
    int **matrix = allocate_matrix(*rows, *cols);
    if (matrix == NULL) {
        return NULL;
    }
    printf("Enter matrix elements row by row:\n");
    for (int i = 0; i < *rows; i++) {
        for (int j = 0; j < *cols; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                free_matrix(matrix, *rows);
                return NULL;
            }
        }
    }
    return matrix;
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
                if ((result[i][j] > 0 && product > INT_MAX - result[i][j]) ||
                    (result[i][j] < 0 && product < INT_MIN - result[i][j])) {
                    free_matrix(result, rows_a);
                    return NULL;
                }
                result[i][j] += (int)product;
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
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[j][i] = matrix[i][j];
        }
    }