//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: matrix_ops
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

enum operation {
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_TRANSPOSE,
    OP_DETERMINANT
};

void print_matrix(int **matrix, int rows, int cols) {
    if (matrix == NULL || rows <= 0 || cols <= 0) return;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int **allocate_matrix(int rows, int cols) {
    if (rows <= 0 || cols <= 0) return NULL;
    int **matrix = malloc(rows * sizeof(int *));
    if (matrix == NULL) return NULL;
    for (int i = 0; i < rows; i++) {
        matrix[i] = malloc(cols * sizeof(int));
        if (matrix[i] == NULL) {
            for (int j = 0; j < i; j++) free(matrix[j]);
            free(matrix);
            return NULL;
        }
    }
    return matrix;
}

void free_matrix(int **matrix, int rows) {
    if (matrix == NULL) return;
    for (int i = 0; i < rows; i++) free(matrix[i]);
    free(matrix);
}

int **read_matrix(int *rows, int *cols) {
    printf("Enter number of rows and columns: ");
    if (scanf("%d %d", rows, cols) != 2 || *rows <= 0 || *cols <= 0) return NULL;
    int **matrix = allocate_matrix(*rows, *cols);
    if (matrix == NULL) return NULL;
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

int **matrix_add(int **a, int **b, int rows, int cols) {
    if (a == NULL || b == NULL || rows <= 0 || cols <= 0) return NULL;
    int **result = allocate_matrix(rows, cols);
    if (result == NULL) return NULL;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = a[i][j] + b[i][j];
        }
    }
    return result;
}

int **matrix_subtract(int **a, int **b, int rows, int cols) {
    if (a == NULL || b == NULL || rows <= 0 || cols <= 0) return NULL;
    int **result = allocate_matrix(rows, cols);
    if (result == NULL) return NULL;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = a[i][j] - b[i][j];
        }
    }
    return result;
}

int **matrix_multiply(int **a, int **b, int rows_a, int cols_a, int cols_b) {
    if (a == NULL || b == NULL || rows_a <= 0 || cols_a <= 0 || cols_b <= 0) return NULL;
    int **result = allocate_matrix(rows_a, cols_b);
    if (result == NULL) return NULL;
    for (int i = 0; i < rows_a; i++) {
        for (int j = 0; j < cols_b; j++) {
            result[i][j] = 0;
            for (int k = 0; k < cols_a; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    return result;
}

int **matrix_transpose(int **matrix, int rows, int cols) {
    if (matrix == NULL || rows <= 0 || cols <= 0) return NULL;
    int **result = allocate_matrix(cols, rows);
    if (result == NULL) return NULL;
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            result[i][j] = matrix[j][i];
        }
    }
    return result;
}

int matrix_determinant(int **matrix, int n) {
    if (matrix == NULL || n <= 0) return 0;
    if (n == 1) return matrix[0][0];
    if (n == 2) return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    int det = 0;
    int **submatrix = allocate_matrix(n - 1, n - 1);
    if (submatrix == NULL) return 0;
    for (int x = 0; x < n; x++) {
        int subi = 0;
        for (int i = 1; i < n; i++) {
            int subj = 0;
            for (int j = 0; j < n; j++) {
                if (j == x) continue;