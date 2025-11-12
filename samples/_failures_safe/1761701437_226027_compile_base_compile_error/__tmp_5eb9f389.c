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
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%4d", matrix[i][j]);
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

int **add_matrices(int **a, int **b, int rows, int cols) {
    int **result = allocate_matrix(rows, cols);
    if (!result) return NULL;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = a[i][j] + b[i][j];
        }
    }
    return result;
}

int **subtract_matrices(int **a, int **b, int rows, int cols) {
    int **result = allocate_matrix(rows, cols);
    if (!result) return NULL;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[i][j] = a[i][j] - b[i][j];
        }
    }
    return result;
}

int **multiply_matrices(int **a, int **b, int rows_a, int cols_a, int cols_b) {
    int **result = allocate_matrix(rows_a, cols_b);
    if (!result) return NULL;
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

int **transpose_matrix(int **matrix, int rows, int cols) {
    int **result = allocate_matrix(cols, rows);
    if (!result) return NULL;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[j][i] = matrix[i][j];
        }
    }
    return result;
}

int determinant_2x2(int **matrix) {
    return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
}

int determinant(int **matrix, int size) {
    if (size == 1) return matrix[0][0];
    if (size == 2) return determinant_2x2(matrix);
    int det = 0;
    int sign = 1;
    for (int col = 0; col < size; col++) {
        int **minor = allocate_matrix(size - 1, size - 1);
        if (!minor) return 0;
        for (int i = 1; i < size; i++) {
            int minor_col = 0;
            for (int j = 0; j < size; j++) {
                if (j == col) continue;
                minor[i - 1][minor_col++] = matrix[i][j];
            }
        }
        det += sign * matrix[0][col] * determinant(minor, size - 1);
        sign = -sign;
        free_matrix(minor, size - 1);
    }
    return det;
}

int main(void) {
    int rows_a, cols_a, rows_b, cols_b;
    printf("Enter dimensions of matrix A (rows cols): ");
    if (scanf("%d %d", &rows_a, &cols_a) != 2 || rows_a <= 0 || cols_a <= 0) {
        printf("Invalid matrix dimensions\n");
        return 1;
    }
    int **matrix_a = allocate_matrix(rows_a, cols_a);
    if (!matrix_a) {
        printf("Memory allocation failed\n");
        return 1;
    }
    printf("Enter elements of matrix A (%d x %d):\n", rows_a, cols_a);
    for (int i = 0; i < rows_a; i++) {
        for (int j = 0; j < cols_a; j++) {
            if (scanf("%d", &matrix_a[i][j]) != 1) {
                printf("Invalid input\n");
                free_matrix(matrix_a, rows_a);
                return 1;
            }
        }
    }
    printf("Matrix A:\n");
    print_matrix(matrix_a, rows